#pragma once
#include <RenderCommand.h>
#include <vector>
#include <map>
#include <Mesh.h>
#include <Texture.h>
#include <Sampler.h>
#include <RenderTarget.h>
#include <ConstantBuffers.h>
#include <ResourceIdentifier.h>
#include <RenderStateBlock.h>

enum class ResourceType {
	Mesh,
	Texture,
	Sampler,
	RenderTarget
};

class DeviceBase
{
public:
	DeviceBase() {}
	virtual ~DeviceBase() {}

	virtual void setRenderStateBlock(RenderStateBlock& renderStateBlock) = 0;

	virtual std::vector<MeshResourceIdentifier> requestMeshResources(std::vector<Mesh*>& meshPtrs) = 0;

	virtual void destroyMeshResources(std::vector<MeshResourceIdentifier>& meshIdentifiers) = 0;

	virtual std::vector<TextureResourceIdentifier> requestTextureResources(std::vector<Texture*>& texturePtrs) = 0;

	virtual void destroyTextureResources(std::vector<TextureResourceIdentifier>& textureIdentifiers) = 0;

	virtual std::vector<SamplerResouceIdentifier> requestSamplerResources(std::vector<Sampler*>& samplerPtrs) = 0;
	virtual void destroySamplerResources(std::vector<SamplerResouceIdentifier>& samplerIdentifiers) = 0;

	virtual std::vector<RenderTargetIdentifier> requestRenderTargetResource(std::vector<RenderTarget*>& renderTargetPtrs, std::vector<std::vector<AttachmentEntityIdentifierWrapper>>& attachmentIdentifiers) = 0;

	virtual void destroyRenderTargetResource(std::vector<RenderTargetIdentifier>& renderTargetIdentifiers) = 0;

	virtual std::vector<RenderBufferIdentifier> requestRenderBufferResources(std::vector<RenderBuffer*>& renderBufferPtrs) = 0;

	virtual void destroyRenderBufferResources(std::vector<RenderBufferIdentifier>& renderBufferIdentifiers) = 0;

	virtual void requestConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers) = 0;

	virtual void destroyConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers) = 0;

	virtual void executeCommand(RenderCommand& renderCommand) = 0;

	virtual void activate(RenderTargetIdentifier* rtIdentifier) = 0;

	virtual void clearColor(float r, float g, float b, float a) = 0;

	virtual void blitToWindow() = 0;

	virtual void uploadConstantBufferResource(ConstantBufferType constantBufferType) = 0;

protected:
	RenderTargetIdentifier* m_curRT;

};