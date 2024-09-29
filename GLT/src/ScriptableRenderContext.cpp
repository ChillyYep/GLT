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
void ScriptableRenderContext::drawRenderers(FilterSettings& filterSetting, DrawSettings& drawSetting)
{
	m_filterSetting = filterSetting;

	m_drawSetting = drawSetting;

	m_renderers = SceneManager::getInstance()->filterRenderers(filterSetting.m_renderType);

	std::vector<RendererSortStructure> simpleStructure(m_renderers.size());

	for (int i = 0; i < m_renderers.size(); ++i)
	{
		auto pos = m_renderers[i]->getWorldBound().m_center;
		simpleStructure[i].m_target = m_renderers[i];
		simpleStructure[i].m_sortOrder = glm::length(pos - m_drawSetting.m_cameraPos);
	}

	if (drawSetting.m_sortType == SortType::Near2Far)
	{
		std::sort(simpleStructure.begin(), simpleStructure.end(), sortNear2FarCompare);
	}
	else if (drawSetting.m_sortType == SortType::Far2Near)
	{
		std::sort(simpleStructure.begin(), simpleStructure.end(), sortFar2NearCompare);
	}
	for (int i = 0; i < m_renderers.size(); ++i)
	{
		m_renderers[i] = simpleStructure[i].m_target;
		m_cmdList.drawRenderer(m_renderers[i]);
	}
	scheduleCommandBuffer(m_cmdList);
	m_cmdList.clear();
	submit();
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

void ScriptableRenderContext::blitCurrentRTToWindow()
{
	m_device->blitCurrentRTToWindow();
}