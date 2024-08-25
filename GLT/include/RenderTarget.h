#pragma once
#include <Object.h>
#include <Texture.h>
#include <RenderBuffer.h>
#include <CommonDefine.h>
#include <TextureEnums.h>

struct RenderTargetDescriptor
{
	RenderTargetDescriptor(int width, int height, TextureInternalFormat colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat,
		RenderTextureDepthStencilType stencilInternalFormat,
		TexturePerChannelSize perChannelSize, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, TextureFilterMode textureFilter)
		:m_width(width), m_height(height), m_colorInternalFormat(colorInternalFormat), m_depthInternalFormat(depthInternalFormat), m_stencilInternalFormat(stencilInternalFormat),
		m_perChannelSize(perChannelSize), m_wrapModeS(wrapModeS), m_wrapModeT(wrapModeT), m_textureFilter(textureFilter)
	{}

	int m_width;
	int m_height;
	TextureInternalFormat m_colorInternalFormat;
	RenderTextureDepthStencilType m_depthInternalFormat;
	RenderTextureDepthStencilType m_stencilInternalFormat;
	TexturePerChannelSize m_perChannelSize;
	TextureWrapMode m_wrapModeS;
	TextureWrapMode m_wrapModeT;
	TextureFilterMode m_textureFilter;
};

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
	~RenderTarget() {}

	//virtual void create(bool immediately = true) = 0;
	//virtual void release(bool immediately = true) = 0;

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