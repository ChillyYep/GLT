#pragma once
#include "PassBase.h"
class DrawOpaquePass :public PassBase
{
public:
	DrawOpaquePass() :PassBase() {}
	~DrawOpaquePass() {}

	void prepare() override
	{
		PassBase::prepare();
		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Back;
		m_renderStateBlock.m_depthState.m_writable = true;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;
	}

	void execute() override
	{
		m_context->setRenderStateBlock(m_renderStateBlock);
	}
private:
	RenderStateBlock m_renderStateBlock;
};