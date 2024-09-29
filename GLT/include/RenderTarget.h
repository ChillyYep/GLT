#pragma once
#include <Texture.h>
#include <RenderBuffer.h>
#include <CommonDefine.h>
#include <RenderTargetDescriptor.h>

class AttachmentEntityWrapper
{
public:
	AttachmentEntityWrapper(RenderBuffer* renderBuffer, FBOAttachmentType attachmentType)
	{
		m_attachmentType = attachmentType;
		setRenderBuffer(renderBuffer);
	}
	AttachmentEntityWrapper(Texture* texture, FBOAttachmentType attachmentType)
	{
		m_attachmentType = attachmentType;
		setTexture(texture);
	}
	void setRenderBuffer(RenderBuffer* renderBuffer)
	{
		m_resourceType = FBOAttachmentResourceType::RenderBuffer;
		m_attachmentEntity = renderBuffer;
	}
	void setTexture(Texture* texture)
	{
		m_resourceType = FBOAttachmentResourceType::Texture;
		m_attachmentEntity = texture;
	}
	RenderBuffer* getRenderBuffer() const
	{
		if (m_resourceType == FBOAttachmentResourceType::RenderBuffer)
		{
			return static_cast<RenderBuffer*>(m_attachmentEntity);
		}
		return nullptr;
	}

	Texture* getTexture() const
	{
		if (m_resourceType == FBOAttachmentResourceType::Texture)
		{
			return static_cast<Texture*>(m_attachmentEntity);
		}
		return nullptr;
	}

	FBOAttachmentResourceType getResourceType() const { return m_resourceType; }
	FBOAttachmentType getAttachmentType() const { return m_attachmentType; }
private:
	FBOAttachmentResourceType m_resourceType;
	FBOAttachmentType m_attachmentType;
	void* m_attachmentEntity;
};

class RenderTarget :public Object
{
public:
	RenderTarget(int width, int height, TextureInternalFormat colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat,
		RenderTextureDepthStencilType stencilInternalFormat,
		TexturePerChannelSize perChannelSize, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, TextureFilterMode textureFilter)
		:m_renderTextureDescriptor(RenderTargetDescriptor(width, height, colorInternalFormat, depthInternalFormat, stencilInternalFormat, perChannelSize,
			wrapModeS, wrapModeT, textureFilter)) {}

	RenderTarget(RenderTargetDescriptor renderTargetDesc)
		:m_renderTextureDescriptor(renderTargetDesc) {}
	~RenderTarget() {}

	void addAttachment(AttachmentEntityWrapper wrapper)
	{
		m_attachments.push_back(wrapper);
	}

	std::vector<AttachmentEntityWrapper>& getAttachments() { return m_attachments; }

	__GET_SET_PROPERTY__(RenderTargetDescriptor, RenderTargetDescriptor, m_renderTextureDescriptor)
protected:

	RenderTargetDescriptor m_renderTextureDescriptor;

	std::vector<AttachmentEntityWrapper> m_attachments;
};