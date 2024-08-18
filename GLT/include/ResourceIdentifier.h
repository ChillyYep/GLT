#pragma once
#include <CommonDefine.h>
#include <TextureEnums.h>
#include <RenderBuffer.h>
#include <ResourceIdentifierType.h>

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

ENUM_BINDING_CLASS(MeshResourceIdentifier, ResourceIdentifier, ResourceIdentifierType, ResourceIdentifierType::Mesh, resourceidentifier_traits)
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

ENUM_BINDING_CLASS(TextureResourceIdentifier, ResourceIdentifier, ResourceIdentifierType, ResourceIdentifierType::Texture, resourceidentifier_traits)
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

ENUM_BINDING_CLASS(SamplerResouceIdentifier, ResourceIdentifier, ResourceIdentifierType, ResourceIdentifierType::Sampler, resourceidentifier_traits)
{
public:
	GLTUInt32 m_sampler;
private:
};

ENUM_BINDING_CLASS(RenderTargetIdentifier, ResourceIdentifier, ResourceIdentifierType, ResourceIdentifierType::RenderTarget, resourceidentifier_traits)
{
public:
	GLTUInt32 m_fbo;

	RenderBuffer m_colorRenderBuffer;
	RenderBuffer m_depthRenderBuffer;
	RenderBuffer m_stencilRenderBuffer;
};
