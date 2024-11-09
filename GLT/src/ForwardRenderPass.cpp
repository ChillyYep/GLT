#include "ForwardRenderPath.h"
void ForwardRenderPath::init(ScriptableRenderContext* renderContext, RenderData* renderData, CommandBuffer* commandBuffer)
{
	RenderPathBase::init(renderContext, renderData, commandBuffer);
	m_globalPassList.push_back(new SimpleShadowMapPass());
	//m_globalPassList.push_back(new CaptureFBOPass());
	m_perCameraPassList.push_back(new DrawOpaquePass());
	m_perCameraPassList.push_back(new DrawSkyboxPass());
	m_perCameraPassList.push_back(new DrawTransparentPass());
	//m_perCameraPassList.push_back(new CaptureFBOPass());
	//m_perCameraPassList.push_back(new PostProcessingPass());
}