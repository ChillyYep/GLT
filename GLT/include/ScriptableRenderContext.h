#pragma once
#include <CommonDefine.h>
#include <iostream>
#include <vector>
#include <GLDevice.h>
#include <CommandBuffer.h>
#include <RenderResourceManager.h>

class ScriptableRenderContext
{
public:
	ScriptableRenderContext() :m_device(nullptr) {}
	~ScriptableRenderContext() {}

	void init();

	void uninit();

	void scheduleCommandBuffer(CommandBuffer commandBuffer);

	void submit();

	void blitToWindow();
private:
	DeviceBase* m_device;
	std::vector<RenderCommand> m_commands;
};