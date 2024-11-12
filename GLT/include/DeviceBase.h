#pragma once
#include <RenderCommand.h>
#include <vector>
#include <map>
#include <Mesh.h>
#include <Texture.h>
#include <Cubemap.h>
#include <Sampler.h>
#include <RenderTarget.h>
#include <ConstantBuffers.h>
#include <ResourceIdentifier.h>
#include <RenderStateBlock.h>
#include <PipelineStateObject.h>

class DeviceBase
{
public:
	DeviceBase() {}
	virtual ~DeviceBase() {}

	virtual void setRenderStateBlock(RenderStateBlock& renderStateBlock) = 0;

	virtual std::vector<MeshResourceIdentifier> requestMeshResources(std::vector<SubMesh*>& meshPtrs) = 0;

	virtual void updateMeshResources(std::vector<SubMesh*>& meshPtrs, std::vector<MeshResourceIdentifier>& meshResourceIdentifiers) = 0;

	virtual void destroyMeshResources(std::vector<MeshResourceIdentifier>& meshIdentifiers) = 0;

	virtual std::vector<TextureResourceIdentifier> requestTextureResources(std::vector<Texture*>& texturePtrs) = 0;

	virtual void updateTextureResources(std::vector<Texture*>& texturePtrs, std::vector<TextureResourceIdentifier>& textureResourceIdentifiers) = 0;

	virtual void destroyTextureResources(std::vector<TextureResourceIdentifier>& textureIdentifiers) = 0;

	virtual std::vector<SamplerResouceIdentifier> requestSamplerResources(std::vector<Sampler*>& samplerPtrs) = 0;
	virtual void destroySamplerResources(std::vector<SamplerResouceIdentifier>& samplerIdentifiers) = 0;

	virtual std::vector<RenderTargetIdentifier> requestRenderTargetResource(std::vector<RenderTarget*>& renderTargetPtrs, std::vector<std::vector<AttachmentEntityIdentifierWrapper>>& attachmentIdentifiers) = 0;

	virtual void destroyRenderTargetResource(std::vector<RenderTargetIdentifier>& renderTargetIdentifiers) = 0;

	virtual std::vector<RenderBufferIdentifier> requestRenderBufferResources(std::vector<RenderBuffer*>& renderBufferPtrs) = 0;

	virtual void destroyRenderBufferResources(std::vector<RenderBufferIdentifier>& renderBufferIdentifiers) = 0;

	virtual void requestConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers) = 0;

	virtual void destroyConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers) = 0;

	virtual void activate(RenderTargetIdentifier* m_shadowMapIdentifier) = 0;

	virtual void clearColor(float r, float g, float b, float a) = 0;

	virtual void blitCurrentRTToWindow() = 0;

	virtual void blitRTToWindow(RenderTargetIdentifier* rt) = 0;

	virtual void uploadConstantBufferResource(ConstantBufferType constantBufferType) = 0;

	virtual void blitDebugRTToWindow() = 0;

	virtual void blitRT(RenderTargetIdentifier* src, RenderTargetIdentifier* dst, FBOAttachmentType attachmentType = FBOAttachmentType::Color) = 0;

	virtual void copyTexture2D(TextureResourceIdentifier* src, TextureResourceIdentifier* dst, int srcLevel, int srcX, int srcY, int dstX, int dstY, int width, int height, int dstLevel) = 0;

	virtual void copyRenderBuffer(RenderBufferIdentifier* src, RenderBufferIdentifier* dst, int srcX, int srcY, int dstX, int dstY, int width, int height) = 0;

	virtual void capture(RenderTargetIdentifier* target, FBOAttachmentType fboAttachmentType, void* pixels, ReadColorChannel colorChannel) = 0;

	virtual void setViewport(int x, int y, int width, int height) = 0;

	virtual void draw(PipelineStateObject& pso) = 0;

	virtual void setViewMatrix(glm::mat4& viewMatrix) = 0;

	virtual void setProjectionMatrix(glm::mat4& projectionMatrix) = 0;

	void release(RenderCommand& command)
	{
		RenderCommandParamFactory::getInstance()->releaseParam(command.param);
		command.param = nullptr;
	}
protected:
	RenderTargetIdentifier* m_curRT;
};