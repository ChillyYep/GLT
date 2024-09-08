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
#include <RenderResourceManagment.h>

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

	void drawRenderers(FilterSettings& filterSetting, DrawSettings& drawSetting);

	void setRenderStateBlock(RenderStateBlock& renderStateBlock)
	{
		m_curRenderStateBlock = renderStateBlock;
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

	void blitToWindow();
private:


	static bool sortNear2FarCompare(RendererSortStructure a, RendererSortStructure b)
	{
		return a.m_sortOrder > b.m_sortOrder;
	}

	static bool sortFar2NearCompare(RendererSortStructure a, RendererSortStructure b)
	{
		return b.m_sortOrder > a.m_sortOrder;
	}

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