#pragma once
#include <PassBase.h>
#include <RenderTexture.h>
#include <iostream>
#include <memory>
#include <CameraOperation.h>
#include <ResourceNames.h>

class ShadowMapPass :public PassBase
{
public:
	ShadowMapPass() {}
	~ShadowMapPass() {}

	bool isExecutable() override { return true; }

	void prepare() override
	{
		PassBase::prepare();
		// prepareRenderState
		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Back;
		//m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(false, false, false, false);
		m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(true, true, true, true);
		m_renderStateBlock.m_depthState.m_depthRange = glm::vec2(0, 1);
		m_renderStateBlock.m_depthState.m_writable = true;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;

		m_filterSettings.m_renderType = RenderType::Opaque;
		m_drawSettings.m_sortType = SortType::Near2Far;

		// prepareResources
		auto window = Window::getInstance();
		auto windowSize = window->getSize();

		m_replacedMaterialPtr = new Material(std::shared_ptr<Shader>(new Shader("shadowmap")));

		m_shadowMapRT = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGBA8, TextureInternalFormat::Depth16,
			TextureInternalFormat::None);
		m_shadowMapRT->m_name = ResourceName::ShadowMapRTName;
		m_shadowMapRT->setColorAttachmentSampleEnabled(true);
		m_shadowMapRT->setDepthAttachmentSampleEnabled(true);
		m_shadowMapRT->create();
	}

	void destroy() override
	{
		if (m_replacedMaterialPtr != nullptr)
		{
			delete m_replacedMaterialPtr;
			m_replacedMaterialPtr = nullptr;
		}
		if (isPrepared())
		{
			m_shadowMapRT->release();
			delete m_shadowMapRT;
			m_shadowMapRT = nullptr;
		}
	}

	void execute() override
	{
		if (m_renderData->m_lightDatas.size() == 0)
		{
			return;
		}
		auto mainLightData = m_renderData->m_lightDatas[0];
		m_context->setRenderStateBlock(m_renderStateBlock);

		m_drawSettings.m_cameraPos = mainLightData.position;
		auto rtIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_shadowMapRT->getRTInstanceId()));
		if (rtIdentifier != nullptr)
		{
			auto window = Window::getInstance();
			auto windowSize = window->getSize();

			auto viewMatrix = Camera::computeViewMatrix(mainLightData.rotation, mainLightData.position);
			auto projectionMatrix = Camera::computeOrthoProjectionMatrix((float)windowSize.x / windowSize.y, m_bound, 1.f, 100.f);
			m_cmdBuffer.setViewport(0, 0, windowSize.x, windowSize.y);
			m_cmdBuffer.setViewMatrix(viewMatrix);
			m_cmdBuffer.setProjectionMatrix(projectionMatrix);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();

			Shader::setGlobalVector(ShaderPropertyNames::EyePosition, glm::vec4(mainLightData.position.x, mainLightData.position.y, mainLightData.position.z, 1.0f));
			m_context->updateConstantBufferResources(ConstantBufferType::PerCamera);

			m_cmdBuffer.setRenderTarget(rtIdentifier);
			m_cmdBuffer.clearColor(0.0f, 0.0f, 0.0f, 0.0f);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
			m_context->drawRenderers(m_filterSettings, m_drawSettings, m_replacedMaterialPtr);

			auto depthTextureAttachmentIdentifier = static_cast<TextureResourceIdentifier*>(rtIdentifier->getAttachmentIdentifier(FBOAttachmentType::Depth, FBOAttachmentResourceType::Texture));
			if (depthTextureAttachmentIdentifier != nullptr)
			{
				m_cmdBuffer.setGlobalTextureResource(depthTextureAttachmentIdentifier);
			}
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
		}
	}
private:
	FilterSettings m_filterSettings;
	DrawSettings m_drawSettings;
	RenderStateBlock m_renderStateBlock;
	RenderTexture* m_shadowMapRT;
	Material* m_replacedMaterialPtr;

	float m_bound = 20.f;
};