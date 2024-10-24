#pragma once
#include <DeviceBase.h>
#include <GLCommon.h>
#include <Window.h>

class GLDevice :public DeviceBase
{
public:
	GLDevice() {}
	~GLDevice() {}

	inline void setTextureWrapMode(GLuint texture, GLenum pname, TextureWrapMode wrapMode)
	{
		switch (wrapMode)
		{
		case TextureWrapMode::ClampEdge:
			glTextureParameteri(texture, pname, GL_CLAMP_TO_EDGE);
			break;
		case TextureWrapMode::Border:
			glTextureParameteri(texture, pname, GL_CLAMP_TO_BORDER);
			break;
		case TextureWrapMode::Repeat:
			glTextureParameteri(texture, pname, GL_REPEAT);
			break;
		case TextureWrapMode::MirroredRepeat:
			glTextureParameteri(texture, pname, GL_MIRRORED_REPEAT);
			break;
		default:
			break;
		}
	}

	inline void setTextureFilter(GLuint texture, GLenum pname, TextureFilterMode textureFilter)
	{
		switch (textureFilter)
		{
		case TextureFilterMode::Point_Mipmap_Point:
			glTextureParameteri(texture, pname, GL_NEAREST_MIPMAP_NEAREST);
			break;
		case TextureFilterMode::Point_Mipmap_Linear:
			glTextureParameteri(texture, pname, GL_NEAREST_MIPMAP_LINEAR);
			break;
		case TextureFilterMode::Linear_Mipmap_Point:
			glTextureParameteri(texture, pname, GL_LINEAR_MIPMAP_NEAREST);
			break;
		case TextureFilterMode::Linear_Mipmap_Linear:
			glTextureParameteri(texture, pname, GL_LINEAR_MIPMAP_LINEAR);
			break;
		default:
			break;
		}
	}

	inline GLenum getGLTextureExternalFormat(TextureExternalFormat externalFormat)
	{
		switch (externalFormat)
		{
		case TextureExternalFormat::RED:
			return GL_RED;
		case TextureExternalFormat::RG:
			return GL_RG;
		case TextureExternalFormat::RGB:
			return GL_RGB;
		case TextureExternalFormat::RGBA:
			return GL_RGBA;
		default:
			break;
		}
		// fallback
		return GL_RGB;
	}

	inline GLenum getGLTextureInternalFormat(TextureInternalFormat internalFormat)
	{
		switch (internalFormat)
		{
		case TextureInternalFormat::None:
			return GL_NONE;
		case TextureInternalFormat::R8:
			return GL_R8;
		case TextureInternalFormat::RG8:
			return GL_RG8;
		case TextureInternalFormat::RGB8:
			return GL_RGB8;
		case TextureInternalFormat::RGBA8:
			return GL_RGBA8;
		case TextureInternalFormat::R16:
			return GL_R16;
		case TextureInternalFormat::RG16:
			return GL_RG16;
		case TextureInternalFormat::RGB16:
			return GL_RGB16;
		case TextureInternalFormat::RGBA16:
			return GL_RGBA16;
		case TextureInternalFormat::R3G3B2:
			return GL_R3_G3_B2;
		case TextureInternalFormat::SRGB8:
			return GL_SRGB8;
		case TextureInternalFormat::SRGBA8:
			return GL_SRGB8_ALPHA8;
		case TextureInternalFormat::RGBA4:
			return GL_RGBA4;
		case TextureInternalFormat::R16F:
			return GL_R16F;
		case TextureInternalFormat::R32F:
			return GL_R32F;
		case TextureInternalFormat::RG16F:
			return GL_RG16F;
		case TextureInternalFormat::RG32F:
			return GL_RG32F;
		case TextureInternalFormat::RGB16F:
			return GL_RGB16F;
		case TextureInternalFormat::RGB32F:
			return GL_RGB32F;
		case TextureInternalFormat::RGBA16F:
			return GL_RGBA16F;
		case TextureInternalFormat::RGBA32F:
			return GL_RGBA32F;
		case TextureInternalFormat::R11F_G11F_B10F:
			return GL_R11F_G11F_B10F;
		case TextureInternalFormat::RGB9_E5:
			return GL_RGB9_E5;
		case TextureInternalFormat::R8_SNORM:
			return GL_R8_SNORM;
		case TextureInternalFormat::R16_SNORM:
			return GL_R16_SNORM;
		case TextureInternalFormat::RG8_SNORM:
			return GL_RG8_SNORM;
		case TextureInternalFormat::RG16_SNORM:
			return GL_RG16_SNORM;
		case TextureInternalFormat::RGB8_SNORM:
			return GL_RGB8_SNORM;
		case TextureInternalFormat::RGB16_SNORM:
			return GL_RGB16_SNORM;
		case TextureInternalFormat::RGBA8_SNORM:
			return GL_RGBA8_SNORM;
		case TextureInternalFormat::RGBA16_SNORM:
			return GL_RGBA16_SNORM;

		case TextureInternalFormat::Depth8:
			return GL_DEPTH_COMPONENT;
		case TextureInternalFormat::Depth16:
			return GL_DEPTH_COMPONENT16;
		case TextureInternalFormat::Depth24:
			return GL_DEPTH_COMPONENT24;
		case TextureInternalFormat::Depth32:
			return GL_DEPTH_COMPONENT32;
		case TextureInternalFormat::Depth32F:
			return GL_DEPTH_COMPONENT32F;
		case TextureInternalFormat::Stencil0:
			return GL_STENCIL_INDEX;
		case TextureInternalFormat::Stencil1:
			return GL_STENCIL_INDEX1;
		case TextureInternalFormat::Stencil4:
			return GL_STENCIL_INDEX4;
		case TextureInternalFormat::Stencil8:
			return GL_STENCIL_INDEX8;
		case TextureInternalFormat::Stencil16:
			return GL_STENCIL_INDEX16;
		case TextureInternalFormat::Depth_Stencil:
			return GL_DEPTH24_STENCIL8;
		default:
			break;
		}
		return GL_NONE;
	}

	inline GLenum getGLTextureChannelSize(TexturePerChannelSize perChannelSize)
	{
		switch (perChannelSize)
		{
		case TexturePerChannelSize::UNSIGNED_BYTE:
			return GL_UNSIGNED_BYTE;
		default:
			break;
		}
		return GL_NONE;
	}

	inline GLenum getGLReadPixelsType(FBOAttachmentType attachmentType, ReadColorChannel colorChannel)
	{
		switch (attachmentType)
		{
		case FBOAttachmentType::Color:
		{
			switch (colorChannel)
			{
			case ReadColorChannel::None:
				return GL_NONE;
			case ReadColorChannel::RED:
				return GL_RED;
			case ReadColorChannel::GREEN:
				return GL_GREEN;
			case ReadColorChannel::BLUE:
				return GL_BLUE;
			case ReadColorChannel::ALPHA:
				return GL_ALPHA;
			case ReadColorChannel::RGB:
				return GL_RGB;
			case ReadColorChannel::RGBA:
				return GL_RGBA;
			default:
				break;
			}
			return GL_RGBA;
		}
		case FBOAttachmentType::Depth:
			return GL_DEPTH_COMPONENT;
		case FBOAttachmentType::Stencil:
			return GL_STENCIL_INDEX;
		case FBOAttachmentType::DepthStencil:
			return GL_DEPTH_COMPONENT;
		default:
			break;
		}
		return GL_NONE;
	}

	inline GLenum getGLBlendMode(BlendMode blendMode)
	{
		switch (blendMode)
		{
		case BlendMode::Zero:
			return GL_ZERO;
		case BlendMode::One:
			return GL_ONE;
		case BlendMode::DstColor:
			return GL_DST_COLOR;
		case BlendMode::SrcColor:
			return GL_SRC_COLOR;
		case BlendMode::DstAlpha:
			return GL_DST_ALPHA;
		case BlendMode::SrcAlpha:
			return GL_SRC_ALPHA;
		case BlendMode::OneMinusDscColor:
			return GL_ONE_MINUS_DST_COLOR;
		case BlendMode::OneMinusSrcColor:
			return GL_ONE_MINUS_SRC_COLOR;
		case BlendMode::OneMinuesDstAlpha:
			return GL_ONE_MINUS_DST_ALPHA;
		case BlendMode::OneMinuesSrcAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;
		case BlendMode::SrcAlphaSaturate:
			// todo
			break;
		default:
			break;
		}
		return GL_ONE;
	}

	void setRenderStateBlock(RenderStateBlock& renderStateBlock) override;

	std::vector<MeshResourceIdentifier> requestMeshResources(std::vector<Mesh*>& meshPtrs) override;
	void updateMeshResources(std::vector<Mesh*>& meshPtrs, std::vector<MeshResourceIdentifier>& meshResourceIdentifiers) override;
	void destroyMeshResources(std::vector<MeshResourceIdentifier>& meshIdentifiers) override;

	std::vector<TextureResourceIdentifier> requestTextureResources(std::vector<Texture*>& texturePtrs) override;
	void updateTextureResources(std::vector<Texture*>& texturePtrs, std::vector<TextureResourceIdentifier>& textureResourceIdentifiers) override;
	void destroyTextureResources(std::vector<TextureResourceIdentifier>& textureIdentifiers) override;

	std::vector<SamplerResouceIdentifier> requestSamplerResources(std::vector<Sampler*>& samplerPtrs) override;
	void destroySamplerResources(std::vector<SamplerResouceIdentifier>& samplerIdentifiers) override;

	std::vector<RenderTargetIdentifier> requestRenderTargetResource(std::vector<RenderTarget*>& renderTargetPtrs, std::vector<std::vector<AttachmentEntityIdentifierWrapper>>& attachmentIdentifiers) override;

	void destroyRenderTargetResource(std::vector<RenderTargetIdentifier>& renderTargetIdentifiers) override;

	std::vector<RenderBufferIdentifier> requestRenderBufferResources(std::vector<RenderBuffer*>& renderBufferPtrs) override;

	void destroyRenderBufferResources(std::vector<RenderBufferIdentifier>& renderBufferIdentifiers) override;

	void requestConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers) override;

	void destroyConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers) override;

	void activate(RenderTargetIdentifier* rtIdentifier) override;

	void clearColor(float r, float g, float b, float a) override;

	void blitCurrentRTToWindow() override;

	void blitDebugRTToWindow() override;

	void blitRTToWindow(RenderTargetIdentifier* rt) override;

	void blitRT(RenderTargetIdentifier* src, RenderTargetIdentifier* dst, FBOAttachmentType attachmentType = FBOAttachmentType::Color) override;

	void initDefaultFBO()
	{
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glFramebufferTexture2D()
	}

	void copyTexture2D(TextureResourceIdentifier* src, TextureResourceIdentifier* dst, int srcLevel, int srcX, int srcY, int dstLevel, int dstX, int dstY, int width, int height) override
	{
		if (src == nullptr || dst == nullptr)
		{
			return;
		}
		glCopyImageSubData(src->m_texture, GL_TEXTURE_2D, srcLevel, srcX, srcY, 0, dst->m_texture, GL_TEXTURE_2D, dstLevel, dstX, dstY, 0, width, height, 1);
	}

	void copyRenderBuffer(RenderBufferIdentifier* src, RenderBufferIdentifier* dst, int srcX, int srcY, int dstX, int dstY, int width, int height) override
	{
		if (src == nullptr || dst == nullptr)
		{
			return;
		}
		glCopyImageSubData(src->m_renderBuffer, GL_RENDERBUFFER, 0, srcX, srcY, 0, dst->m_renderBuffer, GL_RENDERBUFFER, 0, dstX, dstY, 0, width, height, 1);
	}

	void capture(RenderTargetIdentifier* target, FBOAttachmentType fboAttachmentType, void* pixels, ReadColorChannel colorChannel = ReadColorChannel::None) override
	{
		if (target == nullptr || pixels == nullptr)
		{
			return;
		}
		for (const auto& attachmentIdentifier : target->m_attachmentIdentifiers)
		{
			if (attachmentIdentifier.getAttachmentType() == fboAttachmentType)
			{
				if (attachmentIdentifier.getResourceType() == FBOAttachmentResourceType::Texture)
				{
					auto textureIdentifier = attachmentIdentifier.getTextureIdentifier();
					if (textureIdentifier == nullptr)
					{
						continue;
					}
					glBindFramebuffer(GL_READ_FRAMEBUFFER, target->m_fbo);
					glReadPixels(0, 0, textureIdentifier->m_width, textureIdentifier->m_height, getGLReadPixelsType(fboAttachmentType, colorChannel), getGLTextureChannelSize(textureIdentifier->m_perChannelSize), pixels);
					break;
				}
				else if (attachmentIdentifier.getResourceType() == FBOAttachmentResourceType::RenderBuffer)
				{
					auto renderBufferIdentifier = attachmentIdentifier.getRenderBufferIdentifier();
					if (renderBufferIdentifier == nullptr)
					{
						continue;
					}
					//glReadBuffer(GL_BACK);
					glBindFramebuffer(GL_READ_FRAMEBUFFER, target->m_fbo);
					glReadPixels(0, 0, renderBufferIdentifier->m_width, renderBufferIdentifier->m_height, getGLReadPixelsType(fboAttachmentType, colorChannel), GL_UNSIGNED_BYTE, pixels);
					break;
				}
			}
		}
	}

	GLenum textureType2TextureTarget(TextureType textureType) const;

	void draw(PipelineStateObject& pso);

	void fillNoMaterialProperties(PipelineStateObject& pso, std::string propertyName);

	void setViewport(int x, int y, int width, int height) override
	{
		glViewport(x, y, width, height);
	}

	void uploadConstantBufferResource(ConstantBufferType constantBufferType) override;

	void setViewMatrix(glm::mat4& viewMatrix) override
	{
		Shader::setGlobalMatrix(ShaderPropertyNames::ViewMatrix, viewMatrix);
	}

	void setProjectionMatrix(glm::mat4& projectionMatrix) override
	{
		Shader::setGlobalMatrix(ShaderPropertyNames::ProjectMatrix, projectionMatrix);
	}
private:
	void bindTextureUnit(PipelineStateObject& pso, TextureResourceIdentifier* textureIdentifier);

	void fillShaderProperties(PipelineStateObject& pso);

	void fillMaterialPropertyBlocks(PipelineStateObject& pso);

	void bindBlockForProgram(PipelineStateObject& pso);

	void useProgram(PipelineStateObject& pso);

	void bindMesh(PipelineStateObject& pso);

	void drawElements(PipelineStateObject& pso);
};