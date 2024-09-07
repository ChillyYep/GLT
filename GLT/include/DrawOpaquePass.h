#pragma once
#include <PassBase.h>
#include <RenderTexture.h>
#include <RenderResourceManagment.h>

class DrawOpaquePass :public PassBase
{
public:
	DrawOpaquePass() :PassBase() {}
	~DrawOpaquePass() {}

	void prepareRenderState() override
	{
		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Back;
		m_renderStateBlock.m_depthState.m_writable = true;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;

		m_filterSettings.m_renderType = RenderType::Opaque;

		m_drawSettings.m_sortType = SortType::Near2Far;
	}

	void prepareConstantBuffer() override
	{

	}

	void prepareResources() override
	{
		auto window = Window::getInstance();
		auto windowSize = window->getSize();
		m_renderTexture = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGB8, RenderTextureDepthStencilType::Depth16,
			RenderTextureDepthStencilType::None);
		m_renderTexture->create();
	}

	void destroy() override
	{
		if (IsPrepared())
		{
			m_renderTexture->release();
			delete m_renderTexture;
			m_renderTexture = nullptr;
		}
	}

	void execute() override
	{
		m_context->setRenderStateBlock(m_renderStateBlock);
		// 如果多相机绘制，则相机会在同一帧发生变化，所以需要及时更新
		m_drawSettings.m_cameraPos = m_renderData->m_cameraDatas[m_renderData->m_curRenderingCameraIndex].m_worldPos;

		auto rtIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_renderTexture->getRTInstanceId()));
		if (rtIdentifier != nullptr)
		{
			m_cmdBuffer.setRenderTarget(rtIdentifier);
			m_cmdBuffer.clearColor(0.0f, 0.0f, 0.0f, 0.0f);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
			m_context->drawRenderers(m_filterSettings, m_drawSettings);
		}
	}
private:
	FilterSettings m_filterSettings;
	DrawSettings m_drawSettings;
	RenderStateBlock m_renderStateBlock;
	RenderTexture* m_renderTexture;

};