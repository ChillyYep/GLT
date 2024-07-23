#pragma once
#include <CommonDefine.h>
#include <Object.h>

class FrameBufferObject :public Object
{
public:
	FrameBufferObject(bool enableDepthBuffer, bool enableStencilBuffer)
	{

	}
	~FrameBufferObject() {}
	void Create()
	{
		if (!glIsFramebuffer(m_framebuffer))
		{
			glCreateFramebuffers(1, &m_framebuffer);
		}
	}

	void SetColorAttachment(int index, GLuint texture, GLuint level = 0)
	{
		if (glIsFramebuffer(m_framebuffer))
		{
			glNamedFramebufferTexture(m_framebuffer, GL_COLOR_ATTACHMENT0 + index, texture, level);
		}
	}

	void Destroy()
	{
		if (glIsFramebuffer(m_framebuffer))
		{
			glDeleteFramebuffers(1, &m_framebuffer);
		}
	}

	void Recycle()
	{
		//glInvalidateNamedFramebufferData(m_framebuffer)
	}

private:
	const static int ColorAttachmentsCount = 8;
	GLuint m_colorAttachments[ColorAttachmentsCount];
	GLuint m_framebuffer;
};