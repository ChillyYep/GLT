#pragma once
#include <PassBase.h>
#include <RenderTexture.h>

class DebugPassBase :public PassBase
{
public:

	void prepareResources() override
	{
		auto window = Window::getInstance();
		auto windowSize = window->getSize();
		m_renderTexture = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGB8, RenderTextureDepthStencilType::None,
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
		// 如果多相机绘制，则相机会在同一帧发生变化，所以需要及时更新
		auto rtIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_renderTexture->getRTInstanceId()));
		if (rtIdentifier != nullptr)
		{
			// 1、从另一个图的某通道Blit到当前RT的某通道
			// 2、将当前RT blit到窗口
			m_cmdBuffer.setRenderTarget(rtIdentifier);
			m_context->submit();
		}
	}
private:
	RenderTexture* m_renderTexture;
};