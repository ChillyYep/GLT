#pragma once
#include <Object.h>
#include <iostream>
#include <Texture.h>
#include <RenderTarget.h>
#include <LogicResourceManager.h>

class RenderTexture :public Texture2D
{
public:
	RenderTexture(int width, int height, TextureInternalFormat colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat,
		RenderTextureDepthStencilType stencilInternalFormat = RenderTextureDepthStencilType::None,
		TexturePerChannelSize perChannelSize = TexturePerChannelSize::UNSIGNED_BYTE,
		TextureWrapMode wrapModeS = TextureWrapMode::ClampEdge, TextureWrapMode wrapModeT = TextureWrapMode::ClampEdge,
		TextureFilterMode textureFilter = TextureFilterMode::Linear_Mipmap_Linear)
		:Texture2D(), m_renderTarget(nullptr), m_colorRB(nullptr), m_depthRB(nullptr), m_stencilRB(nullptr)
	{
		m_readWrite = false;
		m_isProxy = false;
		m_levels = 1;
		m_width = width;
		m_height = height;
		m_colorInternalFormat = colorInternalFormat;
		m_depthInternalFormat = depthInternalFormat;
		m_stencilInternalFormat = stencilInternalFormat;
		m_externalFormat = TextureExternalFormat::RGBA;
		m_perChannelSize = perChannelSize;
		m_wrapModeS = wrapModeS;
		m_wrapModeT = wrapModeT;
		m_textureFilter = textureFilter;
	}

	~RenderTexture() {}

	void create()
	{
		if (m_isCreated)
		{
			return;
		}
		m_isCreated = true;
		if (m_renderTarget != nullptr)
		{
			return;
		}
		RenderTargetDescriptor rtDesc(m_width, m_height, m_colorInternalFormat, m_depthInternalFormat, m_stencilInternalFormat, m_perChannelSize, m_wrapModeS, m_wrapModeT, m_textureFilter);
		m_renderTarget = LogicResourceManager::getInstance()->addResource(rtDesc);
		if (m_colorInternalFormat != TextureInternalFormat::None)
		{
			RenderBufferDescriptor rbDesc;
			rbDesc.m_width = m_width;
			rbDesc.m_height = m_height;
			rbDesc.m_isDepthBuffer = false;

			m_colorRB = LogicResourceManager::getInstance()->addResource(rbDesc);
			m_colorRB->setColorInternalFormat(m_colorInternalFormat);
			m_renderTarget->addAttachment(AttachmentEntityWrapper(m_colorRB, FBOAttachmentType::Color));
		}
		if (m_depthInternalFormat != RenderTextureDepthStencilType::None)
		{
			RenderBufferDescriptor rbDesc;
			rbDesc.m_width = m_width;
			rbDesc.m_height = m_height;
			rbDesc.m_isDepthBuffer = true;

			m_depthRB = LogicResourceManager::getInstance()->addResource(rbDesc);
			m_depthRB->setDepthStencilType(m_depthInternalFormat);
			m_renderTarget->addAttachment(AttachmentEntityWrapper(m_depthRB,
				m_depthInternalFormat == RenderTextureDepthStencilType::Depth_Stencil ?
				FBOAttachmentType::DepthStencil : FBOAttachmentType::Depth));
		}
		if (m_stencilInternalFormat != RenderTextureDepthStencilType::None)
		{
			RenderBufferDescriptor rbDesc;
			rbDesc.m_width = m_width;
			rbDesc.m_height = m_height;
			rbDesc.m_isDepthBuffer = true;

			m_stencilRB = LogicResourceManager::getInstance()->addResource(rbDesc);
			m_stencilRB->setDepthStencilType(m_stencilInternalFormat);
			m_renderTarget->addAttachment(AttachmentEntityWrapper(m_stencilRB, FBOAttachmentType::Stencil));
		}
		LogicResourceManager::getInstance()->addResource(ResourceType::RenderTarget, m_renderTarget);
	}

	void release()
	{
		if (m_renderTarget == nullptr)
		{
			return;
		}
		if (m_colorRB != nullptr)
		{
			LogicResourceManager::getInstance()->destroyResource(ResourceType::RenderBuffer, m_colorRB);
			m_colorRB = nullptr;
		}
		if (m_depthRB != nullptr)
		{
			LogicResourceManager::getInstance()->destroyResource(ResourceType::RenderBuffer, m_depthRB);
			m_depthRB = nullptr;
		}
		if (m_stencilRB != nullptr)
		{
			LogicResourceManager::getInstance()->destroyResource(ResourceType::RenderBuffer, m_stencilRB);
			m_stencilRB = nullptr;
		}
		LogicResourceManager::getInstance()->destroyResource(ResourceType::RenderTarget, m_renderTarget);
		m_renderTarget = nullptr;
		m_isCreated = false;
	}

	unsigned int getRTInstanceId() { return m_renderTarget != nullptr ? m_renderTarget->getInstanceId() : 0; }
private:
	bool m_isCreated = false;
	RenderTextureDepthStencilType m_depthInternalFormat;
	RenderTextureDepthStencilType m_stencilInternalFormat;
	RenderTarget* m_renderTarget;

	RenderBuffer* m_colorRB;
	RenderBuffer* m_depthRB;
	RenderBuffer* m_stencilRB;
};