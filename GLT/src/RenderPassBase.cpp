#include "RenderPathBase.h"
void RenderPathBase::renderGlobally()
{
	for (const auto& pass : m_globalPassList)
	{
		if (!pass->isInit())
		{
			pass->setup(m_renderContext, m_renderData);
		}
		pass->execute();
	}
}
void RenderPathBase::renderPerCamera(CameraData* cameraData)
{
	for (const auto& pass : m_perCameraPassList)
	{
		if (!pass->isInit())
		{
			pass->setup(m_renderContext, m_renderData);
		}
		pass->execute();
	}
}

void RenderPathBase::init(ScriptableRenderContext* renderContext, RenderData* renderData, CommandBuffer* commandBuffer)
{
	m_renderContext = renderContext;
	m_renderData = renderData;
	m_cmd = commandBuffer;
}
void RenderPathBase::uninit()
{
	for (const auto& pass : m_perCameraPassList)
	{
		pass->destroy();
		delete pass;
	}
	for (const auto& pass : m_globalPassList)
	{
		pass->destroy();
		delete pass;
	}
	m_perCameraPassList.clear();
	m_globalPassList.clear();
}