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
		default:
			break;
		}
		return GL_NONE;
	}

	inline GLenum getDepthStencilGLType(RenderTextureDepthStencilType depthStencilType) const
	{
		switch (depthStencilType)
		{
		case RenderTextureDepthStencilType::None:
			return GL_NONE;
		case RenderTextureDepthStencilType::Depth8:
			return GL_DEPTH_COMPONENT;
		case RenderTextureDepthStencilType::Depth16:
			return GL_DEPTH_COMPONENT16;
		case RenderTextureDepthStencilType::Depth24:
			return GL_DEPTH_COMPONENT24;
		case RenderTextureDepthStencilType::Depth32:
			return GL_DEPTH_COMPONENT32;
		case RenderTextureDepthStencilType::Depth32F:
			return GL_DEPTH_COMPONENT32F;
		case RenderTextureDepthStencilType::Stencil0:
			return GL_STENCIL_INDEX;
		case RenderTextureDepthStencilType::Stencil1:
			return GL_STENCIL_INDEX1;
		case RenderTextureDepthStencilType::Stencil4:
			return GL_STENCIL_INDEX4;
		case RenderTextureDepthStencilType::Stencil8:
			return GL_STENCIL_INDEX8;
		case RenderTextureDepthStencilType::Stencil16:
			return GL_STENCIL_INDEX16;
		case RenderTextureDepthStencilType::Depth_Stencil:
			return GL_DEPTH_STENCIL;
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

	std::vector<MeshResourceIdentifier> requestMeshResources(std::vector<Mesh*>& meshPtrs) override;

	void destroyMeshResources(std::vector<MeshResourceIdentifier>& meshIdentifiers) override;

	std::vector<TextureResourceIdentifier> requestTextureResources(std::vector<Texture*>& texturePtrs) override;

	void destroyTextureResources(std::vector<TextureResourceIdentifier>& textureIdentifiers) override;

	std::vector<SamplerResouceIdentifier> requestSamplerResources(std::vector<Sampler*>& samplerPtrs) override;
	void destroySamplerResources(std::vector<SamplerResouceIdentifier>& samplerIdentifiers) override;

	std::vector<RenderTargetIdentifier> requestRenderTargetResource(std::vector<RenderTarget*>& renderTargetPtrs) override;

	void destroyRenderTargetResource(std::vector<RenderTargetIdentifier>& renderTargetIdentifiers) override;

	void requestConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers) override;

	void destroyConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers) override;

	void activate(RenderTargetIdentifier* rtIdentifier) override;

	void clearColor(float r, float g, float b, float a) override;

	void blitToWindow() override;

	GLenum textureType2TextureTarget(TextureType textureType) const;

	void bindBlockForProgram(Shader& shader);

	void drawMesh(Mesh* mesh, Material* material, glm::mat4 modelMatrix, MeshResourceIdentifier* meshResourceIdentifier, std::unordered_map<GLuint, TextureResourceIdentifier>& textureResources);

	void setViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void uploadConstantBufferResource(ConstantBufferType constantBufferType) override
	{
		auto constantBufferSet = Shader::getShaderConstantBufferSet();

		auto identifier = constantBufferSet.getGlobalBufferIdentifier(constantBufferType);
		auto buffer = constantBufferSet.getGlobalByteBuffer(constantBufferType);

		if (identifier != nullptr)
		{
			GLuint ubo = identifier->GetUbo();
			glNamedBufferSubData(ubo, 0, buffer->dataSize(), buffer->data());
		}
	}
	void release(RenderCommand& command)
	{
		RenderCommandParamFactory::getInstance()->releaseParam(command.param);
		command.param = nullptr;
	}
	void executeCommand(RenderCommand& command) override
	{
		auto commandType = command.commandType;
		auto commandParam = command.param;
		switch (commandType)
		{
		case RenderCommandType::SetViewPort:
		{
			auto setViewPortParam = static_cast<SetViewPortParam*>(commandParam);
			setViewport(setViewPortParam->m_x, setViewPortParam->m_y, setViewPortParam->m_width, setViewPortParam->m_height);
		}
		break;
		case RenderCommandType::SetRenderTarget:
		{
			auto setRenderTargetParam = static_cast<SetRenderTargetParam*>(commandParam);
			activate(setRenderTargetParam->identifier);
		}
		break;
		case RenderCommandType::ClearColor:
		{
			auto clearColorParam = static_cast<ClearColorRenderParam*>(commandParam);
			clearColor(clearColorParam->r, clearColorParam->g, clearColorParam->b, clearColorParam->a);
		}
		break;
		case RenderCommandType::DrawMesh:
		{
			auto drawMeshParam = static_cast<DrawMeshParam*>(commandParam);
			drawMesh(drawMeshParam->m_meshPtr, drawMeshParam->m_materialPtr, drawMeshParam->m_modelMatrix, drawMeshParam->m_meshResourceIdentifier, drawMeshParam->m_textureResources);
		}
		break;
		default:
			break;
		}

		release(command);
	}
private:
	RenderTargetIdentifier* m_curRT;

};