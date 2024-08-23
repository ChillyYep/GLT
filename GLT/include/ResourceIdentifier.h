#pragma once
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
	MeshResourceIdentifier() :m_vao(0), m_vbo(0), m_ebo(0) {}
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
	GLTUInt32 m_sampler;
private:
};

RESOUCEIDENTIFIER_CLASS(RenderBufferIdentifier, ResourceIdentifierType::RenderBuffer)
{
public:
	GLTUInt32 m_renderBuffer;
	int m_width;
	int m_height;
	GLTUInt32 m_internalFormat;
};

class AttachmentEntityWrapper
{
public:
	AttachmentEntityWrapper(RenderBufferIdentifier* identifier, FBOAttachmentType fboAttachmentType) {
		SetRenderBufferIdentifier(identifier);
		m_fboAttachmentType = fboAttachmentType;
	}
	AttachmentEntityWrapper(TextureResourceIdentifier* identifier, FBOAttachmentType fboAttachmentType) {
		SetTextureIdentifier(identifier);
		m_fboAttachmentType = fboAttachmentType;
	}
	void SetRenderBufferIdentifier(RenderBufferIdentifier* identifier)
	{
		m_resourceType = FBOAttachmentResourceType::RenderBuffer;
		m_identifier = identifier;
	}

	void SetTextureIdentifier(TextureResourceIdentifier* identifier)
	{
		m_resourceType = FBOAttachmentResourceType::Texture;
		m_identifier = identifier;
	}

	RenderBufferIdentifier* GetRenderBufferIdentifier() const
	{
		if (m_resourceType == FBOAttachmentResourceType::RenderBuffer)
		{
			return static_cast<RenderBufferIdentifier*>(m_identifier);
		}
		return nullptr;
	}

	TextureResourceIdentifier* GetTextureIdentifier() const
	{
		if (m_resourceType == FBOAttachmentResourceType::Texture)
		{
			return static_cast<TextureResourceIdentifier*>(m_identifier);
		}
		return nullptr;
	}

	FBOAttachmentResourceType GetResourceType() const
	{
		return m_resourceType;
	}
private:
	FBOAttachmentResourceType m_resourceType;
	FBOAttachmentType m_fboAttachmentType;
	ResourceIdentifier* m_identifier;
};

RESOUCEIDENTIFIER_CLASS(RenderTargetIdentifier, ResourceIdentifierType::RenderTarget)
{
public:
	GLTUInt32 m_fbo;

	std::vector<AttachmentEntityWrapper> m_attachments;
};
