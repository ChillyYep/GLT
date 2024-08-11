#pragma once
#include <CommonDefine.h>
#include <iostream>
#include <vector>
#include <GLDevice.h>
#include <CommandBuffer.h>
#include <ResourceManager.h>

class ScriptableRenderContext
{
public:
	ScriptableRenderContext() {}
	~ScriptableRenderContext() {}

	void init()
	{
		m_device = new GLDevice();
		ResourceManager::getInstance()->SetDevice(m_device);
	}

	void uninit()
	{
		if (m_device != nullptr)
		{
			delete m_device;
			m_device = nullptr;
		}
	}

	void scheduleCommandBuffer(CommandBuffer commandBuffer)
	{
		auto& commands = commandBuffer.getAllCmdList();
		for (int i = 0;i < commands.size();++i)
		{
			m_commands.push_back(commands[i]);
		}
	}

	void submit()
	{
		for (int i = 0;i < m_commands.size();++i)
		{
			m_device->executeCommand(m_commands[i]);
		}
	}

	void blitToWindow()
	{
		m_device->blitToWindow();
	}


private:
	DeviceBase* m_device;
	std::vector<RenderCommand> m_commands;
};