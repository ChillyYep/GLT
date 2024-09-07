#include "ScriptableRenderContext.h"

void ScriptableRenderContext::init()
{
	m_device = new GLDevice();
	RenderResourceManagement::getInstance()->setup(m_device);
	RenderResourceManagement::getInstance()->regist();
}

void ScriptableRenderContext::uninit()
{
	RenderResourceManagement::getInstance()->unregist();
	if (m_device != nullptr)
	{
		delete m_device;
		m_device = nullptr;
	}
}

void ScriptableRenderContext::scheduleCommandBuffer(CommandBuffer commandBuffer)
{
	auto& commands = commandBuffer.getAllCmdList();
	for (int i = 0; i < commands.size(); ++i)
	{
		m_commands.push_back(commands[i]);
	}
}

void ScriptableRenderContext::submit()
{
	for (int i = 0; i < m_commands.size(); ++i)
	{
		m_device->executeCommand(m_commands[i]);
	}
	m_commands.clear();
}

void ScriptableRenderContext::blitToWindow()
{
	m_device->blitToWindow();
}