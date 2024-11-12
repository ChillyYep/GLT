#pragma once
#include <PassBase.h>
#include <RenderTexture.h>
#include <iostream>
#include <memory>
#include <CameraOperation.h>
#include <ResourceNames.h>

class SimpleShadowMapPass :public PassBase
{
public:
	SimpleShadowMapPass() {}
	~SimpleShadowMapPass() {}

private:
	bool isExecutable() override { return true; }

	void onDefine() override
	{
		PassBase::onDefine();
		// 渲染ShadowMap以物体背面深度可以有更好的效果
		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Front;
		// 渲染ShadowMap不需要写入颜色
		m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(false, false, false, false);
		//m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(true, true, true, true);
		m_renderStateBlock.m_depthState.m_depthRange = glm::vec2(0, 1);
		m_renderStateBlock.m_depthState.m_writable = true;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;

		m_filterSettings.m_renderType = RenderType::Opaque;
		m_drawSettings.m_sortType = SortType::Near2Far;
	}

	void onPrepare() override
	{
		PassBase::onPrepare();

		m_replacedShadowMaterialPtr = new Material(std::shared_ptr<Shader>(new Shader("SimpleShadowMap")));

		auto shadowData = m_renderData->m_shadowData;
		m_shadowMapRT = new RenderTexture(shadowData.m_shadowSize.x, shadowData.m_shadowSize.y, TextureInternalFormat::None, TextureInternalFormat::Depth16,
			TextureInternalFormat::None, TexturePerChannelSize::UNSIGNED_BYTE, TextureWrapMode::Border, TextureWrapMode::Border, TextureFilterMode::Point_Mipmap_Point, TextureFilterMode::Point_Mipmap_Point);
		m_shadowMapRT->setDepthBorderColor(glm::vec4(1.0f));
		m_shadowMapRT->m_name = ResourceName::ShadowMapRTName;
		m_shadowMapRT->setColorAttachmentSampleEnabled(true);
		m_shadowMapRT->setDepthAttachmentSampleEnabled(true);
		m_shadowMapRT->create();
	}

	bool isPrepared() override
	{
		m_shadowMapIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_shadowMapRT->getRTInstanceId()));
		return m_shadowMapIdentifier != nullptr;
	}

	void onDestroy() override
	{
		if (m_replacedShadowMaterialPtr != nullptr)
		{
			delete m_replacedShadowMaterialPtr;
			m_replacedShadowMaterialPtr = nullptr;
		}
		m_shadowMapRT->release();
		delete m_shadowMapRT;
		m_shadowMapRT = nullptr;
	}

	void onExecute() override
	{
		if (m_renderData->m_lightDatas.size() == 0)
		{
			return;
		}
		auto mainLightData = m_renderData->m_lightDatas[0];
		auto shadowData = m_renderData->m_shadowData;
		m_context->setRenderStateBlock(m_renderStateBlock);

		m_drawSettings.m_cameraPos = mainLightData.position;
		if (m_shadowMapIdentifier != nullptr)
		{
			m_cmdBuffer.setViewport(0, 0, shadowData.m_shadowSize.x, shadowData.m_shadowSize.y);
			m_cmdBuffer.setViewMatrix(shadowData.m_shadowViewMatrix);
			m_cmdBuffer.setProjectionMatrix(shadowData.m_shadowProjectionMatrix);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();

			m_context->updateConstantBufferResources(ConstantBufferType::PerCamera);

			m_cmdBuffer.setRenderTarget(m_shadowMapIdentifier);
			m_cmdBuffer.clearColor(0.0f, 0.0f, 0.0f, 0.0f);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
			m_context->drawRenderers(m_filterSettings, m_drawSettings, m_replacedShadowMaterialPtr);

			auto depthTextureAttachmentIdentifier = static_cast<TextureResourceIdentifier*>(m_shadowMapIdentifier->getAttachmentIdentifier(FBOAttachmentType::Depth, FBOAttachmentResourceType::Texture));
			if (depthTextureAttachmentIdentifier != nullptr)
			{
				m_cmdBuffer.setGlobalTextureResource(ShaderPropertyNames::ShadowMapTex, depthTextureAttachmentIdentifier);
			}

			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
		}
	}

	RenderTargetIdentifier* m_shadowMapIdentifier;

	FilterSettings m_filterSettings;
	DrawSettings m_drawSettings;
	RenderStateBlock m_renderStateBlock;
	RenderTexture* m_shadowMapRT;
	Material* m_replacedShadowMaterialPtr;

};