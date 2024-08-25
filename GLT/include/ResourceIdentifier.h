#pragma once
#include <vector>
#include <CommonDefine.h>
#include <TextureEnums.h>
#include <RenderBuffer.h>
#include <ResourceIdentifierType.h>

enum class FBOAttachmentType {
	Color,
	Depth,
	Stencil,
	DepthStencil
};
enum class FBOAttachmentResourceType {
	Texture,
	RenderBuffer
};
class ResourceIdentifier
{
public:
	ResourceIdentifier() {}
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
	MeshResourceIdentifier() :ResourceIdentifier(0) {}
	MeshResourceIdentifier(GLTUInt32 vao, GLTUInt32 vbo, GLTUInt32 ebo, GLTUInt32 instanceId) :m_vao(vao), m_vbo(vbo), m_ebo(ebo), ResourceIdentifier(instanceId) {}
	~MeshResourceIdentifier() {}
	inline GLTUInt32 getVAO() { return m_vao; }
	inline GLTUInt32 getVBO() { return m_vbo; }
	inline GLTUInt32 getEBO() { return m_ebo; }
	inline GLTUInt32 isValid() { return m_vao > 0; }

private:
	GLTUInt32 m_vao;
	GLTUInt32 m_vbo;
	GLTUInt32 m_ebo;
};

RESOUCEIDENTIFIER_CLASS(TextureResourceIdentifier, ResourceIdentifierType::Texture)
{
public:
	TextureResourceIdentifier() :ResourceIdentifier(0) {}
	TextureResourceIdentifier(GLTUInt32 instanceId) :ResourceIdentifier(instanceId) {}
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
	SamplerResouceIdentifier() :ResourceIdentifier(0) {}
	SamplerResouceIdentifier(GLTUInt32 instanceId) :ResourceIdentifier(instanceId) {}
	~SamplerResouceIdentifier() {}
	GLTUInt32 m_sampler;
private:
};

RESOUCEIDENTIFIER_CLASS(RenderBufferIdentifier, ResourceIdentifierType::RenderBuffer)
{
public:
	RenderBufferIdentifier() :ResourceIdentifier(0) {}
	RenderBufferIdentifier(GLTUInt32 instanceId) :ResourceIdentifier(instanceId) {}
	GLTUInt32 m_renderBuffer;
	bool m_isDepthBuffer;
	int m_width;
	int m_height;
	TextureInternalFormat m_internalFormat;
	RenderTextureDepthStencilType m_depthStencilType;
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
	RenderTargetIdentifier() :ResourceIdentifier(0) {}
	RenderTargetIdentifier(GLTUInt32 instanceId) :ResourceIdentifier(instanceId) {}
	std::vector<AttachmentEntityIdentifierWrapper> m_attachmentIdentifiers;
	GLTUInt32 m_fbo = 0;
private:
};
