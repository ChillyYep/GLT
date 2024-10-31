#pragma once
#include <Object.h>
#include <iostream>
#include <Texture.h>
#include <RenderTarget.h>
#include <LogicResourceManager.h>

class RenderTexture :public Texture2D
{
public:
	RenderTexture(int width, int height, TextureInternalFormat colorInternalFormat, TextureInternalFormat depthInternalFormat,
		TextureInternalFormat stencilInternalFormat = TextureInternalFormat::None,
		TexturePerChannelSize perChannelSize = TexturePerChannelSize::UNSIGNED_BYTE,
		TextureWrapMode wrapModeS = TextureWrapMode::ClampEdge, TextureWrapMode wrapModeT = TextureWrapMode::ClampEdge,
		TextureFilterMode textureFilter = TextureFilterMode::Point_Mipmap_Point, TextureFilterMode depthTextureFilter = TextureFilterMode::Point_Mipmap_Point)
		:Texture2D(), m_colorAttachmentSampleEnabled(false), m_depthAttachmentAllowSampled(false), m_stencilAttachmentAllowSampled(false), m_renderTarget(nullptr),
		m_colorAttachment((RenderBuffer*)nullptr, FBOAttachmentType::Color), m_depthAttachment((RenderBuffer*)nullptr, FBOAttachmentType::Depth), m_stencilAttachment((RenderBuffer*)nullptr, FBOAttachmentType::Stencil)
	{
		m_readWrite = false;
		m_isProxy = false;
		m_levels = 1;
		m_width = width;
		m_height = height;
		m_colorInternalFormat = colorInternalFormat;
		m_depthInternalFormat = depthInternalFormat;
		m_stencilInternalFormat = stencilInternalFormat;
		m_colorExternalFormat = getDefaultTextureExternalFormat(m_colorInternalFormat);
		m_depthExternalFormat = getDefaultTextureExternalFormat(m_depthInternalFormat);
		m_stencilExternalFormat = getDefaultTextureExternalFormat(m_stencilInternalFormat);
		m_perChannelSize = perChannelSize;
		m_wrapModeS = wrapModeS;
		m_wrapModeT = wrapModeT;
		m_textureFilter = textureFilter;
		m_depthTexFilterMode = depthTextureFilter;
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
		RenderTargetDescriptor rtDesc(m_width, m_height, m_colorInternalFormat, m_depthInternalFormat, m_stencilInternalFormat, m_perChannelSize, m_wrapModeS, m_wrapModeT, m_textureFilter, m_depthTexFilterMode);
		m_renderTarget = LogicResourceManager::getInstance()->addResource(rtDesc);
		m_renderTarget->m_name = m_name;
		if (m_colorInternalFormat != TextureInternalFormat::None)
		{
			if (m_colorAttachmentSampleEnabled)
			{
				Texture2D* colorTexture = new Texture2D();
				colorTexture->m_name = m_name;
				colorTexture->setCanReadWrite(true);
				colorTexture->setExternalFormat(m_colorExternalFormat);
				colorTexture->setInternalFormat(m_colorInternalFormat);
				colorTexture->setWidth(m_width);
				colorTexture->setHeight(m_height);
				colorTexture->setLevels(m_levels);
				colorTexture->setIsProxy(m_isProxy);
				colorTexture->setPerChannelSize(m_perChannelSize);
				colorTexture->setTextureFilter(m_textureFilter);
				colorTexture->setWrapModeS(m_wrapModeS);
				colorTexture->setWrapModeT(m_wrapModeT);
				LogicResourceManager::getInstance()->addResource(ResourceType::Texture, colorTexture);
				m_colorAttachment = AttachmentEntityWrapper(colorTexture, FBOAttachmentType::Color);
				m_renderTarget->addAttachment(m_colorAttachment);
			}
			else {
				RenderBufferDescriptor rbDesc;
				rbDesc.m_width = m_width;
				rbDesc.m_height = m_height;
				rbDesc.m_isDepthBuffer = false;
				RenderBuffer* colorRB;
				colorRB = LogicResourceManager::getInstance()->addResource(rbDesc);
				colorRB->setInternalFormat(m_colorInternalFormat);
				m_colorAttachment = AttachmentEntityWrapper(colorRB, FBOAttachmentType::Color);
				m_renderTarget->addAttachment(m_colorAttachment);
			}
		}
		if (m_depthInternalFormat != TextureInternalFormat::None)
		{
			if (m_depthAttachmentAllowSampled)
			{
				Texture2D* depthTexture = new Texture2D();
				depthTexture->m_name = m_name;
				depthTexture->setCanReadWrite(false);
				depthTexture->setExternalFormat(m_depthExternalFormat);
				depthTexture->setInternalFormat(m_depthInternalFormat);
				depthTexture->setWidth(m_width);
				depthTexture->setHeight(m_height);
				depthTexture->setLevels(1);
				depthTexture->setIsProxy(false);
				depthTexture->setPerChannelSize(TexturePerChannelSize::UNSIGNED_BYTE);
				depthTexture->setTextureFilter(m_depthTexFilterMode);
				depthTexture->setWrapModeS(TextureWrapMode::Border);
				depthTexture->setWrapModeT(TextureWrapMode::Border);
				depthTexture->setDepthTexture(true);
				LogicResourceManager::getInstance()->addResource(ResourceType::Texture, depthTexture);
				m_depthAttachment = AttachmentEntityWrapper(depthTexture,
					m_depthInternalFormat == TextureInternalFormat::Depth_Stencil ?
					FBOAttachmentType::DepthStencil : FBOAttachmentType::Depth);
				m_renderTarget->addAttachment(m_depthAttachment);
			}
			else {
				RenderBufferDescriptor rbDesc;
				rbDesc.m_width = m_width;
				rbDesc.m_height = m_height;
				rbDesc.m_isDepthBuffer = true;
				RenderBuffer* depthRB;
				depthRB = LogicResourceManager::getInstance()->addResource(rbDesc);
				depthRB->setInternalFormat(m_depthInternalFormat);

				m_depthAttachment = AttachmentEntityWrapper(depthRB,
					m_depthInternalFormat == TextureInternalFormat::Depth_Stencil ?
					FBOAttachmentType::DepthStencil : FBOAttachmentType::Depth);

				m_renderTarget->addAttachment(m_depthAttachment);
			}
		}
		if (m_stencilInternalFormat != TextureInternalFormat::None)
		{
			if (m_stencilAttachmentAllowSampled)
			{
				Texture2D* stencilTexture = new Texture2D();
				stencilTexture->m_name = m_name;
				stencilTexture->setCanReadWrite(false);
				stencilTexture->setExternalFormat(m_stencilExternalFormat);
				stencilTexture->setInternalFormat(m_stencilInternalFormat);
				stencilTexture->setWidth(m_width);
				stencilTexture->setHeight(m_height);
				stencilTexture->setLevels(1);
				stencilTexture->setIsProxy(false);
				stencilTexture->setPerChannelSize(TexturePerChannelSize::UNSIGNED_BYTE);
				stencilTexture->setTextureFilter(TextureFilterMode::Point_Mipmap_Point);
				stencilTexture->setWrapModeS(TextureWrapMode::ClampEdge);
				stencilTexture->setWrapModeT(TextureWrapMode::ClampEdge);
				LogicResourceManager::getInstance()->addResource(ResourceType::Texture, stencilTexture);
				m_stencilAttachment = AttachmentEntityWrapper(stencilTexture, FBOAttachmentType::Stencil);
				m_renderTarget->addAttachment(m_stencilAttachment);
			}
			else {
				RenderBufferDescriptor rbDesc;
				rbDesc.m_width = m_width;
				rbDesc.m_height = m_height;
				rbDesc.m_isDepthBuffer = true;

				RenderBuffer* stencilRB;
				stencilRB = LogicResourceManager::getInstance()->addResource(rbDesc);
				stencilRB->setInternalFormat(m_stencilInternalFormat);

				m_stencilAttachment = AttachmentEntityWrapper(stencilRB, FBOAttachmentType::Stencil);

				m_renderTarget->addAttachment(m_stencilAttachment);
			}
		}
		LogicResourceManager::getInstance()->addResource(ResourceType::RenderTarget, m_renderTarget);
	}

	void release()
	{
		if (m_renderTarget == nullptr)
		{
			return;
		}
		// ColorAttachment
		release(m_colorAttachment);
		release(m_depthAttachment);
		release(m_stencilAttachment);

		LogicResourceManager::getInstance()->destroyResource(ResourceType::RenderTarget, m_renderTarget);
		m_renderTarget = nullptr;
		m_isCreated = false;
	}

	unsigned int getRTInstanceId() { return m_renderTarget != nullptr ? m_renderTarget->getInstanceId() : 0; }

	void setColorAttachmentSampleEnabled(bool enabled)
	{
		assert(!m_isCreated && "Please call function \"setColorAttachmentSampleEnabled\" before \"create\"!");
		m_colorAttachmentSampleEnabled = enabled;
	}
	void setDepthAttachmentSampleEnabled(bool enabled)
	{
		assert(!m_isCreated && "Please call function \"setColorAttachmentSampleEnabled\" before \"create\"!");
		m_depthAttachmentAllowSampled = enabled;
	}
	void setStencilAttachmentSampleEnabled(bool enabled)
	{
		assert(!m_isCreated && "Please call function \"setColorAttachmentSampleEnabled\" before \"create\"!");
		m_stencilAttachmentAllowSampled = enabled;
	}
private:
	void release(AttachmentEntityWrapper& attachment)
	{
		if (attachment.getResourceType() == FBOAttachmentResourceType::Texture)
		{
			auto texture = attachment.getTexture();
			if (texture != nullptr)
			{
				LogicResourceManager::getInstance()->destroyResource(ResourceType::Texture, texture);
				delete texture;
				attachment.setTexture(nullptr);
			}
		}
		else {
			auto renderBuffer = attachment.getRenderBuffer();
			if (renderBuffer != nullptr)
			{
				LogicResourceManager::getInstance()->destroyResource(ResourceType::RenderBuffer, renderBuffer);
				delete renderBuffer;
				attachment.setRenderBuffer(nullptr);
			}
		}
	}

private:
	bool m_isCreated = false;
	TextureInternalFormat m_depthInternalFormat;
	TextureInternalFormat m_stencilInternalFormat;
	TextureFilterMode m_depthTexFilterMode;
	RenderTarget* m_renderTarget;

	bool m_colorAttachmentSampleEnabled;
	bool m_depthAttachmentAllowSampled;
	bool m_stencilAttachmentAllowSampled;

	AttachmentEntityWrapper m_colorAttachment;

	AttachmentEntityWrapper m_depthAttachment;

	AttachmentEntityWrapper m_stencilAttachment;
};