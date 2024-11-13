#include "ForwardRenderPath.h"
void ForwardRenderPath::init(ScriptableRenderContext* renderContext, RenderData* renderData, CommandBuffer* commandBuffer)
{
	RenderPathBase::init(renderContext, renderData, commandBuffer);
	m_globalPassList.push_back(new ForwardPassSimpleShadowMap());
	//m_globalPassList.push_back(new CaptureFBOPass());
	m_perCameraPassList.push_back(new ForwardPassDrawOpaque());
	m_perCameraPassList.push_back(new ForwardPassDrawSkybox());
	m_perCameraPassList.push_back(new ForwardPassDrawTransparent());
	//m_perCameraPassList.push_back(new CaptureFBOPass());
	m_perCameraPassList.push_back(new ForwardPassScreenSpaceAmbientOcclusion());
}