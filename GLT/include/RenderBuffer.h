#pragma once
#include <CommonDefine.h>
#include <GLCommon.h>

class RenderBuffer
{
public:
	RenderBuffer() {}
	~RenderBuffer() {}
	void create(int width, int height, GLenum internalFormat)
	{
		assert(!glIsRenderbuffer(m_renderBufferPtr));
		m_width = width;
		m_height = height;
		m_internalFormat = internalFormat;
		if (m_internalFormat != GL_NONE)
		{
			// 创建RenderBuffer
			glCreateRenderbuffers(1, &m_renderBufferPtr);
			// 分配RenderBuffer存储空间
			glNamedRenderbufferStorage(m_renderBufferPtr, m_internalFormat, m_width, m_height);
		}
	}

	void release()
	{
		if (glIsRenderbuffer(m_renderBufferPtr))
		{
			glDeleteRenderbuffers(1, &m_renderBufferPtr);
		}
	}
	inline GLuint getRenderBufferPtr()
	{
		return m_renderBufferPtr;
	}
private:
	int m_width;
	int m_height;
	GLenum m_internalFormat;

	GLuint m_renderBufferPtr;
};