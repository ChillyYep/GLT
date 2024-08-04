#pragma once
#include <CommonDefine.h>
#include <TextureEnums.h>

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

class MeshResourceIdentifier :public ResourceIdentifier
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

class TextureResourceIdentifier :public ResourceIdentifier
{
public:
	TextureResourceIdentifier() :ResourceIdentifier(0) {}
	TextureResourceIdentifier(GLTUInt32 instanceId) :ResourceIdentifier(instanceId) {}
	~TextureResourceIdentifier() {}

	inline GLTUInt32 getTextureHandle() { return m_texture; }
	inline TextureType getTextureType() { return m_textureType; }
	friend class ResourceManager;
private:
	GLTUInt32 m_texture;

	/// <summary>
	/// ��������
	/// </summary>
	TextureType m_textureType;
	/// <summary>
	/// �ڲ���ʽ(OpenGLʵ��ʹ��ʱ�ĸ�ʽ)
	/// </summary>
	GLTUInt32 m_internalFormat;

	/// <summary>
	/// �ⲿ��ʽ(����ʱԭʼ�ĸ�ʽ)
	/// </summary>
	GLTUInt32 m_externalFormat;
	/// <summary>
	/// �������ͣ���GL_UNSIGNED_BYTE��
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
