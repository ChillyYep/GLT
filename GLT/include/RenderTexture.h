#pragma once
#include <Object.h>
#include <RenderBuffer.h>
#include <Window.h>

class RenderTexture :public Object
{
public:
	RenderTexture(int width, int height, GLenum colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat, RenderTextureDepthStencilType stencilInternalFormat = RenderTextureDepthStencilType_None)
		:m_width(width), m_height(height), m_colorInternalFormat(colorInternalFormat), m_depthInternalFormat(depthInternalFormat), m_stencilInternalFormat(stencilInternalFormat) {}

	~RenderTexture() {}
	void Create()
	{
		assert(!glIsFramebuffer(m_frameBuffer));

		glCreateFramebuffers(1, &m_frameBuffer);

		if (m_colorInternalFormat != GL_NONE)
		{
			m_colorRenderBuffer.Create(m_width, m_height, m_colorInternalFormat);
			// 将RenderBuffer关联到FrameBuffer的附件
			glNamedFramebufferRenderbuffer(m_frameBuffer, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorRenderBuffer.GetRenderBufferPtr());
		}
		if (m_depthInternalFormat == RenderTextureDepthStencilType::RenderTextureDepthStencilType_Depth_Stencil)
		{
			m_depthRenderBuffer.Create(m_width, m_height, GL_DEPTH_STENCIL);
			// 将RenderBuffer关联到FrameBuffer的附件
			glNamedFramebufferRenderbuffer(m_frameBuffer, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer.GetRenderBufferPtr());
		}
		else
		{
			if (m_depthInternalFormat != RenderTextureDepthStencilType::RenderTextureDepthStencilType_None)
			{
				m_depthRenderBuffer.Create(m_width, m_height, GetDepthStencilGLType(m_depthInternalFormat));
				// 将RenderBuffer关联到FrameBuffer的附件
				glNamedFramebufferRenderbuffer(m_frameBuffer, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer.GetRenderBufferPtr());
			}
			if (m_stencilInternalFormat != RenderTextureDepthStencilType::RenderTextureDepthStencilType_None)
			{
				m_stencilRenderBuffer.Create(m_width, m_height, GetDepthStencilGLType(m_stencilInternalFormat));
				// 将RenderBuffer关联到FrameBuffer的附件
				glNamedFramebufferRenderbuffer(m_frameBuffer, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencilRenderBuffer.GetRenderBufferPtr());
			}
		}
		//glEnable(GL_DEPTH_TEST);
	}

	void Active()
	{
		if (glIsFramebuffer(m_frameBuffer))
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
			s_activedRenderTexture = this;
		}
	}

	void ClearColor()
	{
		auto size = Window::GetInstance().GetSize();
		glViewport(0, 0, size.x, size.y);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void BlitToWindow()
	{
		auto windowSize = Window::GetInstance().GetSize();
		assert(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		// 将FBO绑定到读取FB目标上
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer);
		// 绘制窗口设置为0，意思是重新绑定到窗口的帧缓存上
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		ClearColor();
		glBlitFramebuffer(0, 0, windowSize.x, windowSize.y, 0, 0, windowSize.x, windowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void Release()
	{
		if (glIsFramebuffer(m_frameBuffer))
		{
			glDeleteFramebuffers(1, &m_frameBuffer);
		}
		m_colorRenderBuffer.Release();
		m_depthRenderBuffer.Release();
		m_stencilRenderBuffer.Release();
	}
	inline GLenum GetDepthStencilGLType(RenderTextureDepthStencilType depthStencilType) const
	{
		switch (depthStencilType)
		{
		case RenderTextureDepthStencilType_None:
			return GL_NONE;
		case RenderTextureDepthStencilType_Depth8:
			return GL_DEPTH_COMPONENT;
		case RenderTextureDepthStencilType_Depth16:
			return GL_DEPTH_COMPONENT16;
		case RenderTextureDepthStencilType_Depth24:
			return GL_DEPTH_COMPONENT24;
		case RenderTextureDepthStencilType_Depth32:
			return GL_DEPTH_COMPONENT32;
		case RenderTextureDepthStencilType_Depth32F:
			return GL_DEPTH_COMPONENT32F;
		case RenderTextureDepthStencilType_Stencil0:
			return GL_STENCIL_INDEX;
		case RenderTextureDepthStencilType_Stencil1:
			return GL_STENCIL_INDEX1;
		case RenderTextureDepthStencilType_Stencil4:
			return GL_STENCIL_INDEX4;
		case RenderTextureDepthStencilType_Stencil8:
			return GL_STENCIL_INDEX8;
		case RenderTextureDepthStencilType_Stencil16:
			return GL_STENCIL_INDEX16;
		case RenderTextureDepthStencilType_Depth_Stencil:
			return GL_DEPTH_STENCIL;
		default:
			break;
		}
		return GL_NONE;
	}
	inline static RenderTexture* GetActiveRenderTexture() { return s_activedRenderTexture; }
private:
	static RenderTexture* s_activedRenderTexture;
	int m_width;
	int m_height;
	GLenum m_colorInternalFormat;
	RenderTextureDepthStencilType m_depthInternalFormat;
	RenderTextureDepthStencilType m_stencilInternalFormat;
	GLuint m_frameBuffer;
	RenderBuffer m_colorRenderBuffer;
	RenderBuffer m_depthRenderBuffer;
	RenderBuffer m_stencilRenderBuffer;
	glm::vec4 m_clearColor;
};


class RenderTexture_TextureVersion :public Object
{
public:
	RenderTexture_TextureVersion(int width, int height, GLenum colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat, RenderTextureDepthStencilType stencilInternalFormat = RenderTextureDepthStencilType_None)
		:m_width(width), m_height(height), m_colorInternalFormat(colorInternalFormat), m_depthInternalFormat(depthInternalFormat), m_stencilInternalFormat(stencilInternalFormat) {}

	~RenderTexture_TextureVersion() {}
	void Create()
	{
		assert(!glIsFramebuffer(m_frameBuffer));

		glCreateFramebuffers(1, &m_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

		if (m_colorInternalFormat != GL_NONE)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_colorTexturePtr);

			glBindTexture(GL_TEXTURE_2D, m_colorTexturePtr);
			glTexImage2D(GL_TEXTURE_2D, 0, m_colorInternalFormat, m_width, m_height, 0, m_colorInternalFormat, GL_UNSIGNED_BYTE, nullptr);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			// 将Texture关联到FrameBuffer的附件
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexturePtr, 0);
		}
		if (m_depthInternalFormat == RenderTextureDepthStencilType::RenderTextureDepthStencilType_Depth_Stencil)
		{
			auto depthInternalFormat = GetDepthStencilGLType(m_depthInternalFormat);
			glCreateTextures(GL_TEXTURE_2D, 1, &m_depthTexturePtr);
			glBindTexture(GL_TEXTURE_2D, m_depthTexturePtr);
			glTexImage2D(GL_TEXTURE_2D, 0, depthInternalFormat, m_width, m_height, 0, depthInternalFormat, GL_UNSIGNED_BYTE, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexturePtr, 0);
		}
		else
		{
			if (m_depthInternalFormat != RenderTextureDepthStencilType::RenderTextureDepthStencilType_None)
			{
				auto depthInternalFormat = GetDepthStencilGLType(m_depthInternalFormat);
				glCreateTextures(GL_TEXTURE_2D, 1, &m_depthTexturePtr);
				glBindTexture(GL_TEXTURE_2D, m_depthTexturePtr);
				glTexImage2D(GL_TEXTURE_2D, 0, depthInternalFormat, m_width, m_height, 0, depthInternalFormat, GL_UNSIGNED_BYTE, nullptr);
				glBindTexture(GL_TEXTURE_2D, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexturePtr, 0);
			}
			if (m_stencilInternalFormat != RenderTextureDepthStencilType::RenderTextureDepthStencilType_None)
			{
				auto stencilInternalFormat = GetDepthStencilGLType(m_stencilInternalFormat);
				glCreateTextures(GL_TEXTURE_2D, 1, &m_stencilTexturePtr);
				glBindTexture(GL_TEXTURE_2D, m_stencilTexturePtr);
				glTexImage2D(GL_TEXTURE_2D, 0, stencilInternalFormat, m_width, m_height, 0, stencilInternalFormat, GL_UNSIGNED_BYTE, nullptr);
				glBindTexture(GL_TEXTURE_2D, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_stencilTexturePtr, 0);
			}
		}
		auto framebufferstatus = glCheckNamedFramebufferStatus(m_frameBuffer, GL_FRAMEBUFFER);
		//auto framebufferstatus = glCheckFramebufferStatus;
		if (framebufferstatus != GL_FRAMEBUFFER_COMPLETE)
		{
			switch (framebufferstatus)
			{
			case GL_FRAMEBUFFER_UNDEFINED:
				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_UNDEFINED" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
				break;
			default:
				std::cout << "FramebufferErrorStatus:" << framebufferstatus << std::endl;
				break;
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glEnable(GL_TEXTURE_2D);
		//glEnable(GL_DEPTH_TEST);
	}

	void Active()
	{
		if (glIsFramebuffer(m_frameBuffer))
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
			s_activedRenderTexture = this;
		}
	}

	void ClearColor()
	{
		auto size = Window::GetInstance().GetSize();
		glViewport(0, 0, size.x, size.y);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void BlitToWindow()
	{
		auto windowSize = Window::GetInstance().GetSize();
		// 将FBO绑定到读取FB目标上
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer);
		// 绘制窗口设置为0，意思是重新绑定到窗口的帧缓存上
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		ClearColor();
		glBlitFramebuffer(0, 0, windowSize.x, windowSize.y, 0, 0, windowSize.x, windowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void Release()
	{
		if (glIsFramebuffer(m_frameBuffer))
		{
			glDeleteFramebuffers(1, &m_frameBuffer);
		}
		if (glIsTexture(m_colorTexturePtr))
		{
			glDeleteTextures(1, &m_colorTexturePtr);
		}
		if (glIsTexture(m_depthTexturePtr))
		{
			glDeleteTextures(1, &m_depthTexturePtr);
		}
		if (glIsTexture(m_stencilTexturePtr))
		{
			glDeleteTextures(1, &m_stencilTexturePtr);
		}
	}
	inline GLenum GetDepthStencilGLType(RenderTextureDepthStencilType depthStencilType) const
	{
		switch (depthStencilType)
		{
		case RenderTextureDepthStencilType_None:
			return GL_NONE;
		case RenderTextureDepthStencilType_Depth8:
			return GL_DEPTH_COMPONENT;
		case RenderTextureDepthStencilType_Depth16:
			return GL_DEPTH_COMPONENT16;
		case RenderTextureDepthStencilType_Depth24:
			return GL_DEPTH_COMPONENT24;
		case RenderTextureDepthStencilType_Depth32:
			return GL_DEPTH_COMPONENT32;
		case RenderTextureDepthStencilType_Depth32F:
			return GL_DEPTH_COMPONENT32F;
		case RenderTextureDepthStencilType_Stencil0:
			return GL_STENCIL_INDEX;
		case RenderTextureDepthStencilType_Stencil1:
			return GL_STENCIL_INDEX1;
		case RenderTextureDepthStencilType_Stencil4:
			return GL_STENCIL_INDEX4;
		case RenderTextureDepthStencilType_Stencil8:
			return GL_STENCIL_INDEX8;
		case RenderTextureDepthStencilType_Stencil16:
			return GL_STENCIL_INDEX16;
		case RenderTextureDepthStencilType_Depth_Stencil:
			return GL_DEPTH_STENCIL;
		default:
			break;
		}
		return GL_NONE;
	}
	inline static RenderTexture_TextureVersion* GetActiveRenderTexture() { return s_activedRenderTexture; }
private:
	static RenderTexture_TextureVersion* s_activedRenderTexture;
	int m_width;
	int m_height;
	GLenum m_colorInternalFormat;
	RenderTextureDepthStencilType m_depthInternalFormat;
	RenderTextureDepthStencilType m_stencilInternalFormat;
	GLuint m_frameBuffer;
	GLuint m_colorTexturePtr;
	GLuint m_depthTexturePtr;
	GLuint m_stencilTexturePtr;

	glm::vec4 m_clearColor;
};