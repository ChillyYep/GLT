#pragma once
#include <CommonDefine.h>
#include <TextureEnums.h>

class ResourceIdentifier
{
public:
	ResourceIdentifier() :m_instanceId(0) {}
	ResourceIdentifier(GLTUInt32 instanceId) :m_instanceId(instanceId) {}
	~ResourceIdentifier() {}
	inline GLTUInt32 GetInstanceId() { return m_instanceId; }
private:
	GLTUInt32 m_instanceId;
};

class MeshResourceIdentifier :public ResourceIdentifier
{
public:
	MeshResourceIdentifier() :m_vao(0), m_vbo(0), m_ebo(0) {}
	MeshResourceIdentifier(GLTUInt32 vao, GLTUInt32 vbo, GLTUInt32 ebo, GLTUInt32 instanceId) :m_vao(vao), m_vbo(vbo), m_ebo(ebo), ResourceIdentifier(instanceId) {}
	~MeshResourceIdentifier() {}
	inline GLTUInt32 GetVAO() { return m_vao; }
	inline GLTUInt32 GetVBO() { return m_vbo; }
	inline GLTUInt32 GetEBO() { return m_ebo; }
	inline GLTUInt32 IsValid() { return m_vao > 0; }

private:
	GLTUInt32 m_vao;
	GLTUInt32 m_vbo;
	GLTUInt32 m_ebo;
};

class TextureResourceIdentifier :public ResourceIdentifier
{
public:
	TextureResourceIdentifier() :ResourceIdentifier(0) {}
	TextureResourceIdentifier(GLTUInt32 instanceId) :ResourceIdentifier(instanceId) {}
	~TextureResourceIdentifier() {}

	inline GLTUInt32 GetTextureHandle() { return m_texture; }
	inline TextureType GetTextureType() { return m_textureType; }
	friend class ResourceManager;
private:
	GLTUInt32 m_texture;

	/// <summary>
	/// 纹理类型
	/// </summary>
	TextureType m_textureType;
	/// <summary>
	/// 内部格式(OpenGL实际使用时的格式)
	/// </summary>
	GLTUInt32 m_internalFormat;

	/// <summary>
	/// 外部格式(加载时原始的格式)
	/// </summary>
	GLTUInt32 m_externalFormat;
	/// <summary>
	/// 数据类型，如GL_UNSIGNED_BYTE等
	/// </summary>
	GLTUInt32 m_perChannelSize;

	GLTBool m_isProxy;
	GLTSizei m_levels;

	GLTSizei m_width;
	GLTSizei m_height;
	GLTSizei m_depth;
};

class SamplerResouceIdentifier :ResourceIdentifier {
public:
	friend class ResourceManager;
private:
	GLTUInt32 m_sampler;
};

class RenderTargetIdentifier :public ResourceIdentifier
{

};
