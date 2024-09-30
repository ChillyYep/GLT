#pragma once
#include <PassBase.h>
#include <RenderTexture.h>
#include <LogicResourceManager.h>

class DebugPass :public PassBase
{
public:
	void prepare() override {
		PassBase::prepare();
		// prepareResources
		auto window = Window::getInstance();
		auto windowSize = window->getSize();
		m_colorRT = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGB8, RenderTextureDepthStencilType::None,
			RenderTextureDepthStencilType::None);
		m_colorRT->m_name = "DebugRT";
		m_colorRT->create();
	}

	void destroy() override
	{
		if (IsPrepared())
		{
			m_colorRT->release();
			delete m_colorRT;
			m_colorRT = nullptr;
		}
	}

	void execute() override
	{
		// 如果多相机绘制，则相机会在同一帧发生变化，所以需要及时更新
		auto debugRTIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_colorRT->getRTInstanceId()));
		auto shadowMapRt = static_cast<RenderTarget*>(LogicResourceManager::getInstance()->getResource(ResourceType::RenderTarget, "ShadowMapRT"));
		auto shadowMapRtIdentifier = shadowMapRt == nullptr ? nullptr : static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, shadowMapRt->getInstanceId()));
		if (debugRTIdentifier != nullptr && shadowMapRtIdentifier != nullptr)
		{
			// 1、从另一个图的某通道Blit到当前RT的某通道
			m_context->blitToRenderBuffer(static_cast<RenderBufferIdentifier*>(shadowMapRtIdentifier->getAttachmentIdentifier(FBOAttachmentType::Color, FBOAttachmentResourceType::RenderBuffer)),
				static_cast<RenderBufferIdentifier*>(debugRTIdentifier->getAttachmentIdentifier(FBOAttachmentType::Color, FBOAttachmentResourceType::RenderBuffer)));
			// 2、将当前RT blit到窗口
			m_cmdBuffer.setRenderTarget(debugRTIdentifier);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
			//m_context->blitCurrentRTToWindow();
		}
	}
private:
	RenderTexture* m_colorRT;
};