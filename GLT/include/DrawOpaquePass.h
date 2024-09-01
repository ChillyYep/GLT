#pragma once
#include <PassBase.h>
#include <RenderTexture.h>
#include <RenderResourceManager.h>

class DrawOpaquePass :public PassBase
{
public:
	DrawOpaquePass() :PassBase() {}
	~DrawOpaquePass() {}

	void prepareRenderState() override
	{
		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Back;
		m_renderStateBlock.m_depthState.m_writable = true;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;
	}

	void prepareConstantBuffer() override
	{

	}

	void prepareResources() override
	{
		auto window = Window::getInstance();
		auto windowSize = window->getSize();
		m_renderTexture = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGB8, RenderTextureDepthStencilType::Depth16,
			RenderTextureDepthStencilType::None);
		m_renderTexture->create();
	}

	void destroy() override
	{
		if (IsPrepared())
		{
			m_renderTexture->release();
			delete m_renderTexture;
			m_renderTexture = nullptr;
		}
	}

	void execute() override
	{
		m_context->setRenderStateBlock(m_renderStateBlock);
		auto& rtManagementCentre = RenderResourceManager::getInstance()->getRenderTargetManagementCentre();
		auto instanceIds = rtManagementCentre.getAllObjectInstanceIds();
		if (instanceIds.size() > 0)
		{
			auto rtIdentifier = RenderResourceManager::getInstance()->getRenderTargetResource(instanceIds[0]);
			if (rtIdentifier != nullptr)
			{
				m_cmdBuffer.setRenderTarget(rtIdentifier);
				m_cmdBuffer.clearColor(0.0f, 0.0f, 0.0f, 0.0f);
				m_context->scheduleCommandBuffer(m_cmdBuffer);
				m_cmdBuffer.clear();
				m_context->submit();
			}
		}
	}
private:
	RenderStateBlock m_renderStateBlock;
	RenderTexture* m_renderTexture;

};