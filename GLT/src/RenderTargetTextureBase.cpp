#include "RenderTargetTextureBase.h"

void RenderTargetTextureBase::release(AttachmentEntityWrapper& attachment)
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

void RenderTargetTextureBase::create()
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

	m_renderTarget = LogicResourceManager::getInstance()->addResource(m_descriptor);
	m_renderTarget->m_name = m_name;
	for (int i = 0;i < m_descriptor.m_colorAttachmentSettings.size();++i)
	{
		auto colorAttachmentSetting = m_descriptor.m_colorAttachmentSettings[i];
		if (colorAttachmentSetting.m_internalFormat == TextureInternalFormat::None)
		{
			continue;
		}
		if (colorAttachmentSetting.m_enableSample)
		{
			Texture2D* colorTexture = new Texture2D();
			colorTexture->m_name = m_name;
			colorTexture->setCanReadWrite(false);
			colorTexture->setExternalFormat(Texture::getDefaultTextureExternalFormat(colorAttachmentSetting.m_internalFormat));
			colorTexture->setInternalFormat(colorAttachmentSetting.m_internalFormat);
			colorTexture->setWidth(m_descriptor.m_width);
			colorTexture->setHeight(m_descriptor.m_height);
			colorTexture->setLevels(colorAttachmentSetting.m_levels);
			colorTexture->setIsProxy(false);
			colorTexture->setPerChannelSize(colorAttachmentSetting.m_perChannelSize);
			colorTexture->setTextureFilter(colorAttachmentSetting.m_textureFilter);
			colorTexture->setWrapModeS(colorAttachmentSetting.m_wrapModeS);
			colorTexture->setWrapModeT(colorAttachmentSetting.m_wrapModeT);
			colorTexture->setBorderColor(colorAttachmentSetting.m_borderColor);
			LogicResourceManager::getInstance()->addResource(ResourceType::Texture, colorTexture);
			auto colorAttachment = AttachmentEntityWrapper(colorTexture, FBOAttachmentType::Color);
			m_renderTarget->addAttachment(colorAttachment);
		}
		else {
			RenderBufferDescriptor rbDesc;
			rbDesc.m_width = m_descriptor.m_width;
			rbDesc.m_height = m_descriptor.m_height;
			rbDesc.m_isDepthBuffer = false;
			RenderBuffer* colorRB;
			colorRB = LogicResourceManager::getInstance()->addResource(rbDesc);
			colorRB->setInternalFormat(colorAttachmentSetting.m_internalFormat);
			auto colorAttachment = AttachmentEntityWrapper(colorRB, FBOAttachmentType::Color);
			m_renderTarget->addAttachment(colorAttachment);
		}
	}
	if (m_descriptor.m_depthAttachmentSetting.m_internalFormat != TextureInternalFormat::None)
	{
		auto depthAttachmentSetting = m_descriptor.m_depthAttachmentSetting;
		if (depthAttachmentSetting.m_enableSample)
		{
			Texture2D* depthTexture = new Texture2D();
			depthTexture->m_name = m_name;
			depthTexture->setCanReadWrite(false);
			depthTexture->setExternalFormat(Texture::getDefaultTextureExternalFormat(depthAttachmentSetting.m_internalFormat));
			depthTexture->setInternalFormat(depthAttachmentSetting.m_internalFormat);
			depthTexture->setWidth(m_descriptor.m_width);
			depthTexture->setHeight(m_descriptor.m_height);
			depthTexture->setLevels(depthAttachmentSetting.m_levels);
			depthTexture->setIsProxy(false);
			depthTexture->setPerChannelSize(depthAttachmentSetting.m_perChannelSize);
			depthTexture->setTextureFilter(depthAttachmentSetting.m_textureFilter);
			depthTexture->setWrapModeS(depthAttachmentSetting.m_wrapModeS);
			depthTexture->setWrapModeT(depthAttachmentSetting.m_wrapModeT);
			depthTexture->setBorderColor(depthAttachmentSetting.m_borderColor);
			depthTexture->setDepthTexture(true);
			LogicResourceManager::getInstance()->addResource(ResourceType::Texture, depthTexture);
			auto depthAttachment = AttachmentEntityWrapper(depthTexture,
				depthAttachmentSetting.m_internalFormat == TextureInternalFormat::Depth_Stencil ?
				FBOAttachmentType::DepthStencil : FBOAttachmentType::Depth);
			m_renderTarget->addAttachment(depthAttachment);
		}
		else {
			RenderBufferDescriptor rbDesc;
			rbDesc.m_width = m_descriptor.m_width;
			rbDesc.m_height = m_descriptor.m_height;
			rbDesc.m_isDepthBuffer = true;
			RenderBuffer* depthRB;
			depthRB = LogicResourceManager::getInstance()->addResource(rbDesc);
			depthRB->setInternalFormat(depthAttachmentSetting.m_internalFormat);

			auto depthAttachment = AttachmentEntityWrapper(depthRB,
				depthAttachmentSetting.m_internalFormat == TextureInternalFormat::Depth_Stencil ?
				FBOAttachmentType::DepthStencil : FBOAttachmentType::Depth);

			m_renderTarget->addAttachment(depthAttachment);
		}
	}
	if (m_descriptor.m_stencilAttachmentSetting.m_internalFormat != TextureInternalFormat::None)
	{
		auto stencilAttachmentSetting = m_descriptor.m_stencilAttachmentSetting;
		if (stencilAttachmentSetting.m_enableSample)
		{
			Texture2D* stencilTexture = new Texture2D();
			stencilTexture->m_name = m_name;
			stencilTexture->setCanReadWrite(false);
			stencilTexture->setExternalFormat(Texture::getDefaultTextureExternalFormat(stencilAttachmentSetting.m_internalFormat));
			stencilTexture->setInternalFormat(stencilAttachmentSetting.m_internalFormat);
			stencilTexture->setWidth(m_descriptor.m_width);
			stencilTexture->setHeight(m_descriptor.m_height);
			stencilTexture->setLevels(stencilAttachmentSetting.m_levels);
			stencilTexture->setIsProxy(false);
			stencilTexture->setPerChannelSize(stencilAttachmentSetting.m_perChannelSize);
			stencilTexture->setTextureFilter(stencilAttachmentSetting.m_textureFilter);
			stencilTexture->setWrapModeS(stencilAttachmentSetting.m_wrapModeS);
			stencilTexture->setWrapModeT(stencilAttachmentSetting.m_wrapModeT);
			LogicResourceManager::getInstance()->addResource(ResourceType::Texture, stencilTexture);
			auto stencilAttachment = AttachmentEntityWrapper(stencilTexture, FBOAttachmentType::Stencil);
			m_renderTarget->addAttachment(stencilAttachment);
		}
		else {
			RenderBufferDescriptor rbDesc;
			rbDesc.m_width = m_descriptor.m_width;
			rbDesc.m_height = m_descriptor.m_height;
			rbDesc.m_isDepthBuffer = true;

			RenderBuffer* stencilRB;
			stencilRB = LogicResourceManager::getInstance()->addResource(rbDesc);
			stencilRB->setInternalFormat(stencilAttachmentSetting.m_internalFormat);

			auto stencilAttachment = AttachmentEntityWrapper(stencilRB, FBOAttachmentType::Stencil);
			m_renderTarget->addAttachment(stencilAttachment);
		}
	}
	LogicResourceManager::getInstance()->addResource(ResourceType::RenderTarget, m_renderTarget);
}

void RenderTargetTextureBase::release()
{
	if (m_renderTarget == nullptr)
	{
		return;
	}
	// ColorAttachment
	auto attachments = m_renderTarget->getAttachments();
	for (int i = 0;i < attachments.size();++i)
	{
		release(attachments[i]);
	}

	LogicResourceManager::getInstance()->destroyResource(ResourceType::RenderTarget, m_renderTarget);
	m_renderTarget = nullptr;
	m_isCreated = false;
}
