#pragma once
#include <Object.h>
#include <Window.h>
#include <iostream>
#include <RenderTarget.h>
#include <ResourceManager.h>

class RenderTexture :public RenderTarget
{
public:
	RenderTexture(int width, int height, TextureInternalFormat colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat, RenderTextureDepthStencilType stencilInternalFormat = RenderTextureDepthStencilType::None)
		:RenderTarget(width, height, colorInternalFormat, depthInternalFormat, stencilInternalFormat) {}

	~RenderTexture() {}

	void create(bool immediately = true) override
	{
		if (immediately)
		{

		}
		else {
			auto& rtManagementCentre = ResourceManager::getInstance()->getRenderTargetManagementCentre();
			rtManagementCentre.add(std::shared_ptr<RenderTarget>(this));
		}
	}

	void release(bool immediately = true) override
	{
		if (immediately)
		{

		}
		else {
			auto& rtManagementCentre = ResourceManager::getInstance()->getRenderTargetManagementCentre();
			rtManagementCentre.remove(std::shared_ptr<RenderTarget>(this));
		}
	}

private:
};


//class RenderTexture_TextureVersion :public Object
//{
//public:
//	RenderTexture_TextureVersion(int width, int height, GLenum colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat, RenderTextureDepthStencilType stencilInternalFormat = RenderTextureDepthStencilType::None)
//		:m_width(width), m_height(height), m_colorInternalFormat(colorInternalFormat), m_depthInternalFormat(depthInternalFormat), m_stencilInternalFormat(stencilInternalFormat) {}
//
//	~RenderTexture_TextureVersion() {}
//	void create()
//	{
//		assert(!glIsFramebuffer(m_frameBuffer));
//
//		glCreateFramebuffers(1, &m_frameBuffer);
//		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
//
//		if (m_colorInternalFormat != GL_NONE)
//		{
//			glCreateTextures(GL_TEXTURE_2D, 1, &m_colorTexturePtr);
//
//			glBindTexture(GL_TEXTURE_2D, m_colorTexturePtr);
//			glTexImage2D(GL_TEXTURE_2D, 0, m_colorInternalFormat, m_width, m_height, 0, m_colorInternalFormat, GL_UNSIGNED_BYTE, nullptr);
//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			glBindTexture(GL_TEXTURE_2D, 0);
//			// 将Texture关联到FrameBuffer的附件
//			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexturePtr, 0);
//		}
//		if (m_depthInternalFormat == RenderTextureDepthStencilType::Depth_Stencil)
//		{
//			auto depthInternalFormat = getDepthStencilGLType(m_depthInternalFormat);
//			glCreateTextures(GL_TEXTURE_2D, 1, &m_depthTexturePtr);
//			glBindTexture(GL_TEXTURE_2D, m_depthTexturePtr);
//			glTexImage2D(GL_TEXTURE_2D, 0, depthInternalFormat, m_width, m_height, 0, depthInternalFormat, GL_UNSIGNED_BYTE, nullptr);
//			glBindTexture(GL_TEXTURE_2D, 0);
//			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexturePtr, 0);
//		}
//		else
//		{
//			if (m_depthInternalFormat != RenderTextureDepthStencilType::None)
//			{
//				auto depthInternalFormat = getDepthStencilGLType(m_depthInternalFormat);
//				glCreateTextures(GL_TEXTURE_2D, 1, &m_depthTexturePtr);
//				glBindTexture(GL_TEXTURE_2D, m_depthTexturePtr);
//				glTexImage2D(GL_TEXTURE_2D, 0, depthInternalFormat, m_width, m_height, 0, depthInternalFormat, GL_UNSIGNED_BYTE, nullptr);
//				glBindTexture(GL_TEXTURE_2D, 0);
//				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexturePtr, 0);
//			}
//			if (m_stencilInternalFormat != RenderTextureDepthStencilType::None)
//			{
//				auto stencilInternalFormat = getDepthStencilGLType(m_stencilInternalFormat);
//				glCreateTextures(GL_TEXTURE_2D, 1, &m_stencilTexturePtr);
//				glBindTexture(GL_TEXTURE_2D, m_stencilTexturePtr);
//				glTexImage2D(GL_TEXTURE_2D, 0, stencilInternalFormat, m_width, m_height, 0, stencilInternalFormat, GL_UNSIGNED_BYTE, nullptr);
//				glBindTexture(GL_TEXTURE_2D, 0);
//				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_stencilTexturePtr, 0);
//			}
//		}
//		auto framebufferstatus = glCheckNamedFramebufferStatus(m_frameBuffer, GL_FRAMEBUFFER);
//		//auto framebufferstatus = glCheckFramebufferStatus;
//		if (framebufferstatus != GL_FRAMEBUFFER_COMPLETE)
//		{
//			switch (framebufferstatus)
//			{
//			case GL_FRAMEBUFFER_UNDEFINED:
//				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_UNDEFINED" << std::endl;
//				break;
//			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
//				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
//				break;
//			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
//				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
//				break;
//			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
//				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
//				break;
//			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
//				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
//				break;
//			case GL_FRAMEBUFFER_UNSUPPORTED:
//				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
//				break;
//			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
//				std::cout << "FramebufferErrorStatus:GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
//				break;
//			default:
//				std::cout << "FramebufferErrorStatus:" << framebufferstatus << std::endl;
//				break;
//			}
//		}
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		//glEnable(GL_TEXTURE_2D);
//		//glEnable(GL_DEPTH_TEST);
//	}
//
//	void activate()
//	{
//		if (glIsFramebuffer(m_frameBuffer))
//		{
//			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
//			s_activedRenderTexture = this;
//		}
//	}
//
//	void clearColor()
//	{
//		auto size = Window::getInstance()->getSize();
//		glViewport(0, 0, size.x, size.y);
//		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	}
//
//	void blitToWindow()
//	{
//		auto windowSize = Window::getInstance()->getSize();
//		// 将FBO绑定到读取FB目标上
//		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBuffer);
//		// 绘制窗口设置为0，意思是重新绑定到窗口的帧缓存上
//		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//		clearColor();
//		glBlitFramebuffer(0, 0, windowSize.x, windowSize.y, 0, 0, windowSize.x, windowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
//	}
//
//	void release()
//	{
//		if (glIsFramebuffer(m_frameBuffer))
//		{
//			glDeleteFramebuffers(1, &m_frameBuffer);
//		}
//		if (glIsTexture(m_colorTexturePtr))
//		{
//			glDeleteTextures(1, &m_colorTexturePtr);
//		}
//		if (glIsTexture(m_depthTexturePtr))
//		{
//			glDeleteTextures(1, &m_depthTexturePtr);
//		}
//		if (glIsTexture(m_stencilTexturePtr))
//		{
//			glDeleteTextures(1, &m_stencilTexturePtr);
//		}
//	}
//
//	inline static GLenum getDepthStencilGLType(RenderTextureDepthStencilType depthStencilType)
//	{
//		switch (depthStencilType)
//		{
//		case RenderTextureDepthStencilType::None:
//			return GL_NONE;
//		case RenderTextureDepthStencilType::Depth8:
//			return GL_DEPTH_COMPONENT;
//		case RenderTextureDepthStencilType::Depth16:
//			return GL_DEPTH_COMPONENT16;
//		case RenderTextureDepthStencilType::Depth24:
//			return GL_DEPTH_COMPONENT24;
//		case RenderTextureDepthStencilType::Depth32:
//			return GL_DEPTH_COMPONENT32;
//		case RenderTextureDepthStencilType::Depth32F:
//			return GL_DEPTH_COMPONENT32F;
//		case RenderTextureDepthStencilType::Stencil0:
//			return GL_STENCIL_INDEX;
//		case RenderTextureDepthStencilType::Stencil1:
//			return GL_STENCIL_INDEX1;
//		case RenderTextureDepthStencilType::Stencil4:
//			return GL_STENCIL_INDEX4;
//		case RenderTextureDepthStencilType::Stencil8:
//			return GL_STENCIL_INDEX8;
//		case RenderTextureDepthStencilType::Stencil16:
//			return GL_STENCIL_INDEX16;
//		case RenderTextureDepthStencilType::Depth_Stencil:
//			return GL_DEPTH_STENCIL;
//		default:
//			break;
//		}
//		return GL_NONE;
//	}
//
//	inline static RenderTexture_TextureVersion* getActiveRenderTexture() { return s_activedRenderTexture; }
//private:
//	static RenderTexture_TextureVersion* s_activedRenderTexture;
//	int m_width;
//	int m_height;
//	GLenum m_colorInternalFormat;
//	RenderTextureDepthStencilType m_depthInternalFormat;
//	RenderTextureDepthStencilType m_stencilInternalFormat;
//	GLuint m_frameBuffer;
//	GLuint m_colorTexturePtr;
//	GLuint m_depthTexturePtr;
//	GLuint m_stencilTexturePtr;
//
//	glm::vec4 m_clearColor;
//};