#pragma once
#include <PassBase.h>
#include <ResourceNames.h>

class ForwardPassDrawTransparent :public PassBase
{
public:
	ForwardPassDrawTransparent() {}
	~ForwardPassDrawTransparent() {}

private:
	bool isExecutable() override { return true; }

	void onDefine() override
	{
		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Off;
		m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(true, true, true, true);
		m_renderStateBlock.m_colorState.m_blendModeEnabled = true;
		m_renderStateBlock.m_colorState.m_srcBlendMode = BlendMode::SrcAlpha;
		m_renderStateBlock.m_colorState.m_dstBlendMode = BlendMode::OneMinuesSrcAlpha;

		m_renderStateBlock.m_depthState.m_writable = false;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;

		m_filterSettings.m_renderType = RenderType::Transparent;
		m_drawSettings.m_sortType = SortType::Far2Near;
	}

	void onPrepare() override {}

	bool isPrepared() override
	{
		m_colorRT = static_cast<RenderTarget*>(LogicResourceManager::getInstance()->getResource(ResourceType::RenderTarget, ResourceName::OpaqueRTName));
		if (m_colorRT == nullptr)
		{
			return false;
		}
		m_colorRTIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_colorRT->getInstanceId()));
		return m_colorRTIdentifier != nullptr;
	}

	void onDestroy() override {}

	void onExecute() override
	{
		m_context->setRenderStateBlock(m_renderStateBlock);
		m_drawSettings.m_cameraPos = m_renderData->m_cameraDatas[m_renderData->m_curRenderingCameraIndex].m_worldPos;
		if (m_colorRT != nullptr && m_colorRTIdentifier != nullptr)
		{
			m_colorRTIdentifier->setColorAttachmentBlocked(1, true);
			m_cmdBuffer.setRenderTarget(m_colorRTIdentifier);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
			m_context->drawRenderers(m_filterSettings, m_drawSettings);
		}
	}

	RenderTarget* m_colorRT;
	RenderTargetIdentifier* m_colorRTIdentifier;

	FilterSettings m_filterSettings;
	DrawSettings m_drawSettings;
	RenderStateBlock m_renderStateBlock;
};
