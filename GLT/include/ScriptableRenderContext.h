#pragma once
#include <CommonDefine.h>
#include <iostream>
#include <vector>
#include <GLDevice.h>
#include <CommandBuffer.h>
#include <CullingResult.h>
#include <FilterSetting.h>
#include <SceneManager.h>
#include <Camera.h>
#include <DrawSettings.h>
#include <algorithm>

class ScriptableRenderContext
{
private:
	/// <summary>
	/// 排序用辅助结构
	/// </summary>
	struct RendererSortStructure {
		Renderer* m_target;
		float m_sortOrder;
	};
public:
	ScriptableRenderContext() :m_device(nullptr) {}
	~ScriptableRenderContext() {}

	void init();

	void uninit();

	void tryGetCullingResult(Camera* camera, CullingResult& cullingResult)
	{
		/*	auto scenes = SceneManager::getInstance()->getAllScenes();
			for (int i = 0;i < scenes.size();++i)
			{

			}*/
	}

	void drawRenderers(FilterSettings& filterSetting, DrawSettings& drawSetting, Material* replacedMaterial = nullptr);

	void setRenderStateBlock(RenderStateBlock& renderStateBlock)
	{
		// 配置相同则不必再刷新
		if (m_renderStateInited && m_curRenderStateBlock.isSame(renderStateBlock))
		{
			return;
		}
		if (!m_renderStateInited)
		{
			glDepthRange(0, 1);
		}
		m_renderStateInited = true;
		renderStateBlock.copyTo(m_curRenderStateBlock);
		m_device->setRenderStateBlock(m_curRenderStateBlock);
	}

	void updateConstantBufferResources(ConstantBufferType constantBufferType)
	{
		m_device->uploadConstantBufferResource(constantBufferType);
	}

	void requestConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers)
	{
		m_device->requestConstantBufferResources(constantBufferIdentifiers);
	}

	void destroyConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers)
	{
		m_device->destroyConstantBufferResources(constantBufferIdentifiers);
	}

	void scheduleCommandBuffer(CommandBuffer commandBuffer);

	void submit();

	void blitCurrentRTToWindow();

	void blitRT(RenderTargetIdentifier* src, RenderTargetIdentifier* dst, FBOAttachmentType attachmentType = FBOAttachmentType::Color)
	{
		m_device->blitRT(src, dst, attachmentType);
	}

	void copyTexture2D(TextureResourceIdentifier* src, TextureResourceIdentifier* dst, int srcLevel, int srcX, int srcY, int dstLevel, int dstX, int dstY, int width, int height)
	{
		m_device->copyTexture2D(src, dst, srcLevel, srcX, srcY, dstLevel, dstX, dstY, width, height);
	}

	void copyRenderbuffer(RenderBufferIdentifier* src, RenderBufferIdentifier* dst, int srcX, int srcY, int dstX, int dstY, int width, int height)
	{
		m_device->copyRenderBuffer(src, dst, srcX, srcY, dstX, dstY, width, height);
	}
	/// <summary>
	/// 从帧缓冲中读取像素到内存
	/// </summary>
	/// <param name="target"></param>
	/// <param name="fboAttachmentType"></param>
	/// <param name="pixels"></param>
	/// <param name="colorChannel"></param>
	void capture(RenderTargetIdentifier* target, FBOAttachmentType fboAttachmentType, void* pixels, ReadColorChannel colorChannel = ReadColorChannel::None)
	{
		m_device->capture(target, fboAttachmentType, pixels, colorChannel);
	}

private:

	static bool sortNear2FarCompare(RendererSortStructure a, RendererSortStructure b)
	{
		return a.m_sortOrder > b.m_sortOrder;
	}

	static bool sortFar2NearCompare(RendererSortStructure a, RendererSortStructure b)
	{
		return b.m_sortOrder > a.m_sortOrder;
	}

	void executeCommand(RenderCommand& command);

	void setupPSO(MeshResourceIdentifier* meshResourceIdentifier, glm::mat4 modelMatrix, Material* material,
		std::vector<TextureResourceIdentifier*> textureResources)
	{
		auto shader = material->getShader().get();
		m_pso.m_meshIdentifier = meshResourceIdentifier;
		m_pso.m_texUnit = 0;
		m_pso.m_material = material;
		m_pso.m_modelMatrix = modelMatrix;
		m_pso.m_textureResources = textureResources;
		m_pso.m_program = shader->getShaderProgram();
		m_pso.m_constantBufferSet = &Shader::getShaderConstantBufferSet();
		m_pso.m_uniforms = shader->getShaderUniforms();
		m_pso.m_uniformBlockRefs = shader->getReferencedBlocks();
	}

	void addGlobalTextureResource(std::string textureName, TextureResourceIdentifier* textureResource)
	{
		m_pso.m_globalTextureResources[textureName] = textureResource;
	}

	PipelineStateObject m_pso;
	/// <summary>
	/// 渲染状态是否初始化过
	/// </summary>
	bool m_renderStateInited = false;
	/// <summary>
	/// 当前渲染状态
	/// </summary>
	RenderStateBlock m_curRenderStateBlock;

	FilterSettings m_filterSetting;

	DrawSettings m_drawSetting;

	/// <summary>
	/// 待渲染目标
	/// </summary>
	std::vector<Renderer*> m_renderers;

	/// <summary>
	/// 待执行指令集合
	/// </summary>
	std::vector<RenderCommand> m_commands;

	CommandBuffer m_cmdList;

	/// <summary>
	/// 图形设备接口
	/// </summary>
	DeviceBase* m_device;

};