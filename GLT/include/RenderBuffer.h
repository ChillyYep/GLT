#pragma once
#include <CommonDefine.h>
class RenderBuffer
{
public:
	RenderBuffer() {}
	~RenderBuffer() {}
	void Create(int width, int height, GLenum internalFormat)
	{
		assert(!glIsRenderbuffer(m_renderBufferPtr));
		m_width = width;
		m_height = height;
		m_internalFormat = internalFormat;
		if (m_internalFormat != GL_NONE)
		{
			// ����RenderBuffer
			glCreateRenderbuffers(1, &m_renderBufferPtr);
			// ����RenderBuffer�洢�ռ�
			glNamedRenderbufferStorage(m_renderBufferPtr, m_internalFormat, m_width, m_height);
		}
	}

	void Release()
	{
		if (glIsRenderbuffer(m_renderBufferPtr))
		{
			glDeleteRenderbuffers(1, &m_renderBufferPtr);
		}
	}
	inline GLuint GetRenderBufferPtr()
	{
		return m_renderBufferPtr;
	}
private:
	int m_width;
	int m_height;
	GLenum m_internalFormat;

	GLuint m_renderBufferPtr;
};