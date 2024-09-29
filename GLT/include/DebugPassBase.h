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
		// �����������ƣ����������ͬһ֡�����仯��������Ҫ��ʱ����
		auto rtIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_renderTexture->getRTInstanceId()));
		if (rtIdentifier != nullptr)
		{
			// 1������һ��ͼ��ĳͨ��Blit����ǰRT��ĳͨ��
			// 2������ǰRT blit������
			m_cmdBuffer.setRenderTarget(rtIdentifier);
			m_context->submit();
		}
	}
private:
	RenderTexture* m_renderTexture;
};