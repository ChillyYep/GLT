#pragma once
#include <CommonDefine.h>
#include <iostream>
#include <vector>
#include <GLDevice.h>
#include <CommandBuffer.h>
#include <RenderResourceManager.h>
#include <CullingResult.h>
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
		//auto scenes = SceneManager::getInstance()->getAllScenes();
		//for (int i = 0;i < scenes.size();++i)
		//{

		//}
	}

	void setRenderStateBlock(RenderStateBlock& renderStateBlock) { m_device->setRenderStateBlock(renderStateBlock); }

	void scheduleCommandBuffer(CommandBuffer commandBuffer);

	void submit();

	void blitToWindow();
private:
	DeviceBase* m_device;
	std::vector<RenderCommand> m_commands;
};