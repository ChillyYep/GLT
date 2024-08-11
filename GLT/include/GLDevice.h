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

	std::vector<MeshResourceIdentifier> requestMeshResources(std::vector<Mesh*>& meshPtrs) override;

	void destroyMeshResources(std::vector<MeshResourceIdentifier>& meshIdentifiers) override;

	std::vector<TextureResourceIdentifier> requestTextureResources(std::vector<Texture*>& texturePtrs) override;

	void destroyTextureResources(std::vector<TextureResourceIdentifier>& textureIdentifiers) override;

	std::vector<SamplerResouceIdentifier> requestSamplerResources(std::vector<Sampler*>& samplerPtrs) override;
	void destroySamplerResources(std::vector<SamplerResouceIdentifier>& samplerIdentifiers) override;

	void requestConstantBufferResource(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers) override;
	void destroyConstantBufferResource(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers) override;

	std::vector<RenderTargetIdentifier> requestRenderTargetResource(std::vector<RenderTarget*>& renderTargetPtrs) override;

	void destroyRenderTargetResource(std::vector<RenderTargetIdentifier>& renderTargetIdentifiers) override;

	void activate(RenderTargetIdentifier& rtIdentifier) override;

	void clearColor(float r, float g, float b, float a) override;

	void blitToWindow() override;

	void executeCommand(RenderCommand& command) override
	{
		auto commandType = command.commandType;
		auto commandParam = command.param;
		switch (commandType)
		{
		case RenderCommandType::SetRenderTarget:
		{
			auto setRenderTargetParam = static_cast<SetRenderTargetParam*>(commandParam);
			activate(*setRenderTargetParam->identifier);
		}
		break;
		case RenderCommandType::ClearColor:
		{
			auto clearColorParam = static_cast<ClearColorRenderParam*>(commandParam);
			clearColor(clearColorParam->r, clearColorParam->g, clearColorParam->b, clearColorParam->a);
		}
			break;
		case RenderCommandType::Draw:
		{

		}
			break;
		default:
			break;
		}
	}
private:
	RenderTargetIdentifier m_curRT;

};