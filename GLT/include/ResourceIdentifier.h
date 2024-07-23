#pragma once
#include <CommonDefine.h>
class ResourceIdentifier
{
public:
	ResourceIdentifier() :m_instanceId(0) {}
	ResourceIdentifier(GLuint instanceId) :m_instanceId(instanceId) {}
	~ResourceIdentifier() {}
	inline GLuint GetInstanceId() { return m_instanceId; }
private:
	GLuint m_instanceId;
};

class MeshResourceIdentifier :ResourceIdentifier
{
public:
	MeshResourceIdentifier() :m_vao(0), m_vbo(0), m_ebo(0) {}
	MeshResourceIdentifier(GLuint vao, GLuint vbo, GLuint ebo, GLuint instanceId) :m_vao(vao), m_vbo(vbo), m_ebo(ebo), ResourceIdentifier(instanceId) {}
	~MeshResourceIdentifier() {}
	inline GLuint GetVAO() { return m_vao; }
	inline GLuint GetVBO() { return m_vbo; }
	inline GLuint GetEBO() { return m_ebo; }
	inline GLboolean IsValid() { return m_vao > 0; }

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
};

class TextureResourceIdentifier :ResourceIdentifier
{
public:
	TextureResourceIdentifier() :ResourceIdentifier(0) {}
	TextureResourceIdentifier(GLuint instanceId) :ResourceIdentifier(instanceId) {}
	~TextureResourceIdentifier() {}

	inline GLuint GetTextureHandle() { return m_texture; }
	inline TextureType GetTextureType() { return m_textureType; }
	friend class ResourceManager;
private:
	GLuint m_texture;

	/// <summary>
	/// ��������
	/// </summary>
	TextureType m_textureType;
	/// <summary>
	/// �ڲ���ʽ(OpenGLʵ��ʹ��ʱ�ĸ�ʽ)
	/// </summary>
	GLenum m_internalFormat;

	/// <summary>
	/// �ⲿ��ʽ(����ʱԭʼ�ĸ�ʽ)
	/// </summary>
	GLenum m_externalFormat;
	/// <summary>
	/// �������ͣ���GL_UNSIGNED_BYTE��
	/// </summary>
	GLenum m_perChannelSize;

	GLboolean m_isProxy;
	GLsizei m_levels;

	GLsizei m_width;
	GLsizei m_height;
	GLsizei m_depth;
};

class SamplerResouceIdentifier :ResourceIdentifier {
public:
	friend class ResourceManager;
private:
	GLuint m_sampler;
};
