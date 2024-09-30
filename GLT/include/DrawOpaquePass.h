#pragma once
#include <PassBase.h>
#include <RenderTexture.h>
#include <RenderResourceManagment.h>

class DrawOpaquePass :public PassBase
{
public:
	DrawOpaquePass() :PassBase() {}
	~DrawOpaquePass() {}
	void prepare() override
	{
		PassBase::prepare();
		// prepareRenderState
		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Back;
		m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(true, true, true, true);
		m_renderStateBlock.m_depthState.m_depthRange = glm::ivec2(0, 1);
		m_renderStateBlock.m_depthState.m_writable = true;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;

		m_filterSettings.m_renderType = RenderType::Opaque;
		m_drawSettings.m_sortType = SortType::Near2Far;

		// prepareResources
		auto window = Window::getInstance();
		auto windowSize = window->getSize();
		m_colorRT = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGB8, RenderTextureDepthStencilType::Depth16,
			RenderTextureDepthStencilType::None);
		m_colorRT->m_name = "OpaqueRT";
		m_colorRT->create();
	}

	void destroy() override
	{
		if (IsPrepared())
		{
			m_colorRT->release();
			delete m_colorRT;
			m_colorRT = nullptr;
		}
	}

	void execute() override
	{
		m_context->setRenderStateBlock(m_renderStateBlock);
		// �����������ƣ����������ͬһ֡�����仯��������Ҫ��ʱ����
		m_drawSettings.m_cameraPos = m_renderData->m_cameraDatas[m_renderData->m_curRenderingCameraIndex].m_worldPos;
		auto rtIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_colorRT->getRTInstanceId()));
		if (rtIdentifier != nullptr)
		{
			m_cmdBuffer.setRenderTarget(rtIdentifier);
			m_cmdBuffer.clearColor(0.0f, 0.0f, 0.0f, 0.0f);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
			m_context->drawRenderers(m_filterSettings, m_drawSettings);
		}
	}
private:
	FilterSettings m_filterSettings;
	DrawSettings m_drawSettings;
	RenderStateBlock m_renderStateBlock;
	RenderTexture* m_colorRT;

};