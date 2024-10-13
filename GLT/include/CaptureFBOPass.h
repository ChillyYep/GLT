#pragma once
#include <PassBase.h>
#include <RenderTexture.h>
#include <LogicResourceManager.h>
#include <ResourceNames.h>

class CaptureFBOPass :public PassBase
{
public:
	bool isExecutable() override { return true; }

	void prepare() override {
		PassBase::prepare();

		define();

	}

	virtual void define()
	{
		defineShadowMapCaptureSetting();
	}

	void defineShadowMapCaptureSetting()
	{
		m_readChannels = 1;
		m_debugInternalFormat = TextureInternalFormat::R8;
		m_debugRTName = ResourceName::CaptureFBODebugRTName;
		m_targetRTName = ResourceName::ShadowMapRTName;
		m_targetAttachmentType = FBOAttachmentType::Depth;
		m_targetColorChannel = ReadColorChannel::None;
	}

	void defineOpauqeRTDepthSetting()
	{
		m_readChannels = 1;
		m_debugInternalFormat = TextureInternalFormat::R16;
		m_debugRTName = ResourceName::CaptureFBODebugRTName;
		m_targetRTName = ResourceName::OpaqueRTName;
		m_targetAttachmentType = FBOAttachmentType::DepthStencil;
		m_targetColorChannel = ReadColorChannel::None;
	}

	void ensureCreateDebugColorTexture(int width, int height)
	{
		if (m_debugViewRT == nullptr || m_debugViewRT->getWidth() != width || m_debugViewRT->getHeight() != height)
		{
			ensureReleaseDebugColorTexture();
			m_debugViewRT = new RenderTexture(width, height, m_debugInternalFormat, TextureInternalFormat::None,
				TextureInternalFormat::None);
			m_debugViewRT->m_name = m_debugRTName;
			m_debugViewRT->setColorAttachmentSampleEnabled(true);
			m_debugViewRT->create();

			m_debugColorTexture = getDebugColorTexture();
		}
	}

	void ensureReleaseDebugColorTexture()
	{
		if (m_debugColorTexture != nullptr)
		{
			m_debugColorTexture->setData(nullptr);
			m_debugColorTexture = nullptr;
		}
		if (m_debugViewRT != nullptr)
		{
			m_debugViewRT->release();
			delete m_debugViewRT;
			m_debugViewRT = nullptr;
		}
	}

	Texture* getDebugColorTexture()
	{
		if (m_debugViewRT == nullptr)
		{
			return nullptr;
		}
		Texture* targetTexture = nullptr;
		auto debugRenderTarget = static_cast<RenderTarget*>(LogicResourceManager::getInstance()->getResource(ResourceType::RenderTarget, m_debugViewRT->getRTInstanceId()));
		if (debugRenderTarget != nullptr)
		{
			for (const auto& attachment : debugRenderTarget->getAttachments())
			{
				if (attachment.getAttachmentType() == FBOAttachmentType::Color && attachment.getResourceType() == FBOAttachmentResourceType::Texture)
				{
					targetTexture = attachment.getTexture();
					break;
				}
			}
		}
		return targetTexture;
	}

	void destroy() override
	{
		if (!isPrepared())
		{
			return;
		}
		if (m_readPixels != nullptr)
		{
			delete[] m_readPixels;
			m_readPixels = nullptr;
		}
		ensureReleaseDebugColorTexture();
	}

	void execute() override
	{
		// 使用glReadPixels读取某纹理像素，然后用glTextureSubImage2D写进某Texture中，并Blit至窗口来获取debugview
		// 如果多相机绘制，则相机会在同一帧发生变化，所以需要及时更新
		auto targetRt = static_cast<RenderTarget*>(LogicResourceManager::getInstance()->getResource(ResourceType::RenderTarget, m_targetRTName));
		auto targetRtIdentifier = targetRt == nullptr ? nullptr : static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, targetRt->getInstanceId()));
		if (targetRt != nullptr && targetRtIdentifier != nullptr)
		{
			ensureCreateDebugColorTexture(targetRt->getRenderTargetDescriptor().m_width, targetRt->getRenderTargetDescriptor().m_height);
		}
		if (m_debugViewRT == nullptr)
		{
			return;
		}
		auto debugRTIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_debugViewRT->getRTInstanceId()));
		if (m_debugColorTexture != nullptr && debugRTIdentifier != nullptr)
		{
			auto size = targetRtIdentifier->m_descriptor.m_width * targetRtIdentifier->m_descriptor.m_height * m_readChannels;
			// 第一次
			if (m_readPixels == nullptr)
			{
				m_width = targetRtIdentifier->m_descriptor.m_width;
				m_height = targetRtIdentifier->m_descriptor.m_height;
				m_readPixels = new GLTUByte[size];
				memset(m_readPixels, 0, size);
			}
			else
			{
				if (m_width != targetRtIdentifier->m_descriptor.m_width ||
					m_height != targetRtIdentifier->m_descriptor.m_height)
				{
					m_width = targetRtIdentifier->m_descriptor.m_width;
					m_height = targetRtIdentifier->m_descriptor.m_height;
					delete[] m_readPixels;
					m_readPixels = new GLTUByte[size];
				}
			}
			//m_cmdBuffer.setRenderTarget(targetRtIdentifier);
			//m_context->scheduleCommandBuffer(m_cmdBuffer);
			//m_cmdBuffer.clear();
			//m_context->submit();
			//m_context->blitCurrentRTToWindow();
			// 获取Pixels
			m_context->capture(targetRtIdentifier, m_targetAttachmentType, m_readPixels, m_targetColorChannel);
			// 填充Pixels
			m_debugColorTexture->setData(m_readPixels, size);
			m_cmdBuffer.setRenderTarget(debugRTIdentifier);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
		}
	}
private:
	Texture* m_debugColorTexture;

	RenderTexture* m_debugViewRT;

	GLTUByte* m_readPixels = nullptr;

	int m_width, m_height;

	TextureInternalFormat m_debugInternalFormat;

	std::string m_debugRTName;

	std::string m_targetRTName;

	FBOAttachmentType m_targetAttachmentType;

	ReadColorChannel m_targetColorChannel;

	int m_readChannels;
};