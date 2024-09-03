#pragma once
#include <CommonDefine.h>
#include <iostream>
#include <vector>
#include <GLDevice.h>
#include <CommandBuffer.h>
#include <RenderResourceManager.h>
#include <CullingResult.h>
#include <FilterSetting.h>
#include <SceneManager.h>
#include <Camera.h>
#include <DrawSettings.h>
#include <algorithm>

class ScriptableRenderContext
{
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

	void drawRenderers(FilterSettings& filterSetting, DrawSettings& drawSetting)
	{
		m_filterSetting = filterSetting;

		m_drawSetting = drawSetting;

		m_renderers = SceneManager::getInstance()->filterRenderers(filterSetting.m_renderType);

		if (drawSetting.m_sortType == SortType::Near2Far)
		{
			std::sort(m_renderers.begin(), m_renderers.end(), sortNear2FarCompare);
		}
		else if (drawSetting.m_sortType == SortType::Far2Near)
		{
			std::sort(m_renderers.begin(), m_renderers.end(), sortFar2NearCompare);
		}
	}

	bool sortNear2FarCompare(Renderer* a, Renderer* b)
	{
		auto boundA = a->getWorldBound();
		auto boundB = b->getWorldBound();

		return glm::length(boundA.m_center - m_drawSetting.m_cameraPos) > glm::length(boundB.m_center - m_drawSetting.m_cameraPos);
	}

	bool sortFar2NearCompare(Renderer* a, Renderer* b)
	{
		auto boundA = a->getWorldBound();
		auto boundB = b->getWorldBound();

		return glm::length(boundB.m_center - m_drawSetting.m_cameraPos) > glm::length(boundA.m_center - m_drawSetting.m_cameraPos);
	}

	void setRenderStateBlock(RenderStateBlock& renderStateBlock)
	{
		m_curRenderStateBlock = renderStateBlock;
		m_device->setRenderStateBlock(m_curRenderStateBlock);
	}

	void scheduleCommandBuffer(CommandBuffer commandBuffer);

	void submit();

	void blitToWindow();
private:
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

	/// <summary>
	/// 图形设备接口
	/// </summary>
	DeviceBase* m_device;

};