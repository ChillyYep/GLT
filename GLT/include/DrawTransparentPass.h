#pragma once
#include <PassBase.h>
#include <ResourceNames.h>

class DrawTransparentPass :public PassBase
{
public:
	DrawTransparentPass() {}
	~DrawTransparentPass() {}

	bool isExecutable() override { return true; }

	void prepare() override
	{
		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Off;
		// 渲染ShadowMap不需要写入颜色
		m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(true, true, true, true);
		m_renderStateBlock.m_depthState.m_depthRange = glm::vec2(0, 1);
		m_renderStateBlock.m_depthState.m_writable = false;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;

		m_filterSettings.m_renderType = RenderType::Transparent;
		m_drawSettings.m_sortType = SortType::Far2Near;

	}

	void destroy() override
	{

	}

	void execute() override
	{
		m_context->setRenderStateBlock(m_renderStateBlock);
		m_drawSettings.m_cameraPos = m_renderData->m_cameraDatas[m_renderData->m_curRenderingCameraIndex].m_worldPos;
		auto colorRT = static_cast<RenderTarget*>(LogicResourceManager::getInstance()->getResource(ResourceType::RenderTarget, ResourceName::OpaqueRTName));
		if (colorRT == nullptr)
		{
			return;
		}
		auto colorRTIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, colorRT->getInstanceId()));
		if (colorRTIdentifier != nullptr)
		{
			m_context->drawRenderers(m_filterSettings, m_drawSettings);
		}
	}
private:

	FilterSettings m_filterSettings;
	DrawSettings m_drawSettings;
	RenderStateBlock m_renderStateBlock;
};
