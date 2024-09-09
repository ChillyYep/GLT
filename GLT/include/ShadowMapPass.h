#pragma once
#include <PassBase.h>
#include <RenderTexture.h>
#include <iostream>

class ShadowMapPass :public PassBase
{
public:
	ShadowMapPass() {}
	~ShadowMapPass() {}

	void prepareRenderState() override
	{
		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Back;
		m_renderStateBlock.m_depthState.m_writable = true;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;

		m_filterSettings.m_renderType = RenderType::Opaque;
		m_drawSettings.m_sortType = SortType::Near2Far;
	}

	void prepareResources() override
	{
		auto window = Window::getInstance();
		auto windowSize = window->getSize();
		m_depthRT = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGBA8, RenderTextureDepthStencilType::Depth16,
			RenderTextureDepthStencilType::None);
		m_depthRT->create();
	}

	void destroy() override
	{
		if (IsPrepared())
		{
			m_depthRT->release();
			delete m_depthRT;
			m_depthRT = nullptr;
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
		auto rtIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_depthRT->getRTInstanceId()));
		if (rtIdentifier != nullptr)
		{
			auto viewMatrix = Camera::computeViewMatrix(glm::quatLookAt(mainLightData.direction, glm::vec3(0.0f, 1.0f, 0.0f)), mainLightData.position);
			auto pos = glm::inverse(viewMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			std::cout << mainLightData.position.x << "," << mainLightData.position.y << "," << mainLightData.position.z << std::endl;
			std::cout << pos.x << "," << pos.y << "," << pos.z << std::endl;
			std::cout << std::endl;
			m_cmdBuffer.setViewMatrix(viewMatrix);
			m_cmdBuffer.setProjectionMatrix(m_renderData->m_cameraDatas[0].m_projectionMatrix);
			m_cmdBuffer.setRenderTarget(rtIdentifier);
			m_cmdBuffer.clearColor(0.0f, 0.0f, 0.0f, 0.0f);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();

			m_context->updateConstantBufferResources(ConstantBufferType::PerCamera);
			m_context->drawRenderers(m_filterSettings, m_drawSettings);
		}
	}
private:
	FilterSettings m_filterSettings;
	DrawSettings m_drawSettings;
	RenderStateBlock m_renderStateBlock;
	RenderTexture* m_depthRT;
};