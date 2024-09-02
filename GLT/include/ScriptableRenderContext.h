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

	void filterRenderers(FilterSettings& filterSetting)
	{
		m_renderers = SceneManager::getInstance()->filterRenderers(filterSetting.m_renderType);
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
	/// ��ǰ��Ⱦ״̬
	/// </summary>
	RenderStateBlock m_curRenderStateBlock;

	/// <summary>
	/// ����ȾĿ��
	/// </summary>
	std::vector<Renderer*> m_renderers;

	/// <summary>
	/// ��ִ��ָ���
	/// </summary>
	std::vector<RenderCommand> m_commands;

	/// <summary>
	/// ͼ���豸�ӿ�
	/// </summary>
	DeviceBase* m_device;

};