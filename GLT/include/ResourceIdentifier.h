#pragma once
#include <vector>
#include <CommonDefine.h>
#include <RenderTargetDescriptor.h>
#include <RenderBuffer.h>
#include <ResourceIdentifierType.h>

enum class FBOAttachmentType {
	Color = 1,
	Depth = 2,
	Stencil = 4,
	DepthStencil = 8
};
enum class FBOAttachmentResourceType {
	Texture,
	RenderBuffer
};
class ResourceIdentifier
{
public:
	ResourceIdentifier() :m_instanceId(0) {}
	ResourceIdentifier(GLTUInt32 instanceId) :m_instanceId(instanceId) {}
	~ResourceIdentifier() {}
	inline GLTUInt32 getInstanceId() { return m_instanceId; }
private:
	GLTUInt32 m_instanceId;
};

#define RESOUCEIDENTIFIER_CLASS(CLASSTYPE,CLASSTYPEENUM) ENUM_BINDING_CLASS(CLASSTYPE,ResourceIdentifier,ResourceIdentifierType,CLASSTYPEENUM,resourceidentifier_traits)

RESOUCEIDENTIFIER_CLASS(MeshResourceIdentifier, ResourceIdentifierType::Mesh)
{
public:
	MeshResourceIdentifier(GLTUInt32 vao, GLTUInt32 vbo, GLTUInt32 ebo, GLTUInt32 instanceId,
		GLTUInt32 verticesCount, GLTUInt32 indicesCount) :m_vao(vao), m_vbo(vbo), m_ebo(ebo), m_verticesCount(verticesCount), m_indicesCount(indicesCount),
		ResourceIdentifier(instanceId) {}
	MeshResourceIdentifier() :MeshResourceIdentifier(0, 0, 0, 0, 0, 0) {}
	~MeshResourceIdentifier() {}
	inline GLTUInt32 getVAO() { return m_vao; }
	inline GLTUInt32 getVBO() { return m_vbo; }
	inline GLTUInt32 getEBO() { return m_ebo; }

	inline GLTUInt32 getVerticesCount() { return m_verticesCount; }
	inline GLTUInt32 getIndicesCount() { return m_indicesCount; }
	inline GLTUInt32 isValid() { return m_vao > 0; }

private:
	GLTUInt32 m_vao;
	GLTUInt32 m_vbo;
	GLTUInt32 m_ebo;

	GLTUInt32 m_verticesCount;
	GLTUInt32 m_indicesCount;
};

RESOUCEIDENTIFIER_CLASS(TextureResourceIdentifier, ResourceIdentifierType::Texture)
{
public:
	TextureResourceIdentifier(GLTUInt32 instanceId)
		:ResourceIdentifier(instanceId), m_texture(0), m_textureType(TextureType::Texture1D), m_internalFormat(TextureInternalFormat::None),
		m_externalFormat(TextureExternalFormat::RED), m_perChannelSize(TexturePerChannelSize::UNSIGNED_BYTE),
		m_width(0), m_height(0), m_depth(0), m_levels(0), m_isProxy(false)
	{}
	TextureResourceIdentifier() :TextureResourceIdentifier(0) {}
	~TextureResourceIdentifier() {}

	GLTUInt32 m_texture;

	/// <summary>
	/// 纹理类型
	/// </summary>
	TextureType m_textureType;
	/// <summary>
	/// 内部格式(OpenGL实际使用时的格式)
	/// </summary>
	TextureInternalFormat m_internalFormat;

	/// <summary>
	/// 外部格式(加载时原始的格式)
	/// </summary>
	TextureExternalFormat m_externalFormat;
	/// <summary>
	/// 数据类型，如GL_UNSIGNED_BYTE等
	/// </summary>
	TexturePerChannelSize m_perChannelSize;

	GLTBool m_isProxy;
	GLTSizei m_levels;

	GLTSizei m_width;
	GLTSizei m_height;
	GLTSizei m_depth;
private:
};

RESOUCEIDENTIFIER_CLASS(SamplerResouceIdentifier, ResourceIdentifierType::Sampler)
{
public:
	SamplerResouceIdentifier(GLTUInt32 instanceId) :ResourceIdentifier(instanceId), m_sampler(0) {}
	SamplerResouceIdentifier() :SamplerResouceIdentifier(0) {}
	~SamplerResouceIdentifier() {}
	GLTUInt32 m_sampler;
private:
};

RESOUCEIDENTIFIER_CLASS(RenderBufferIdentifier, ResourceIdentifierType::RenderBuffer)
{
public:
	RenderBufferIdentifier(GLTUInt32 instanceId)
		:ResourceIdentifier(instanceId), m_renderBuffer(0), m_isDepthBuffer(false), m_width(0), m_height(0), m_internalFormat(TextureInternalFormat::None)
	{}
	RenderBufferIdentifier() :RenderBufferIdentifier(0) {}
	GLTUInt32 m_renderBuffer;
	bool m_isDepthBuffer;
	int m_width;
	int m_height;
	TextureInternalFormat m_internalFormat;
private:
};

class AttachmentEntityIdentifierWrapper
{
public:
	AttachmentEntityIdentifierWrapper(RenderBufferIdentifier* identifier, FBOAttachmentType fboAttachmentType) {
		setRenderBufferIdentifier(identifier);
		m_fboAttachmentType = fboAttachmentType;
	}
	AttachmentEntityIdentifierWrapper(TextureResourceIdentifier* identifier, FBOAttachmentType fboAttachmentType) {
		setTextureIdentifier(identifier);
		m_fboAttachmentType = fboAttachmentType;
	}
	void setRenderBufferIdentifier(RenderBufferIdentifier* identifier)
	{
		m_resourceType = FBOAttachmentResourceType::RenderBuffer;
		m_identifier = identifier;
	}

	void setTextureIdentifier(TextureResourceIdentifier* identifier)
	{
		m_resourceType = FBOAttachmentResourceType::Texture;
		m_identifier = identifier;
	}

	RenderBufferIdentifier* getRenderBufferIdentifier() const
	{
		if (m_resourceType == FBOAttachmentResourceType::RenderBuffer)
		{
			return static_cast<RenderBufferIdentifier*>(m_identifier);
		}
		return nullptr;
	}

	TextureResourceIdentifier* getTextureIdentifier() const
	{
		if (m_resourceType == FBOAttachmentResourceType::Texture)
		{
			return static_cast<TextureResourceIdentifier*>(m_identifier);
		}
		return nullptr;
	}

	FBOAttachmentResourceType getResourceType() const { return m_resourceType; }

	FBOAttachmentType getAttachmentType() const { return m_fboAttachmentType; }
private:
	FBOAttachmentResourceType m_resourceType;
	FBOAttachmentType m_fboAttachmentType;
	ResourceIdentifier* m_identifier;
};

RESOUCEIDENTIFIER_CLASS(RenderTargetIdentifier, ResourceIdentifierType::RenderTarget)
{
public:
	RenderTargetIdentifier() :ResourceIdentifier(0), m_descriptor(RenderTargetDescriptor()) {}
	RenderTargetIdentifier(GLTUInt32 instanceId) :ResourceIdentifier(instanceId) {}
	std::vector<AttachmentEntityIdentifierWrapper> m_attachmentIdentifiers;
	GLTUInt32 m_fbo = 0;
	RenderTargetDescriptor m_descriptor;

	ResourceIdentifier* getAttachmentIdentifier(FBOAttachmentType attachmentType, FBOAttachmentResourceType resourceType) {
		for (const auto& identifierWrapper : m_attachmentIdentifiers)
		{
			if (identifierWrapper.getAttachmentType() == attachmentType && identifierWrapper.getResourceType() == resourceType)
			{
				if (resourceType == FBOAttachmentResourceType::RenderBuffer)
				{
					return identifierWrapper.getRenderBufferIdentifier();
				}
				else {
					return identifierWrapper.getTextureIdentifier();
				}
			}
		}
		return nullptr;
	}
private:
};
