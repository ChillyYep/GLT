#pragma once
#include <ScriptableRenderContext.h>
#include <RenderData.h>

class PassBase
{
public:
	PassBase() :m_context(nullptr), m_cmdBuffer() {}
	virtual ~PassBase() {}

	/// <summary>
	/// 设置环境
	/// </summary>
	void setup(ScriptableRenderContext* context, RenderData* renderData) { m_context = context; m_renderData = renderData; }

	/// <summary>
	/// 预备
	/// </summary>
	void prepare()
	{
		m_isPrepared = true;
		prepareRenderState();
		prepareConstantBuffer();
		prepareResources();
	}

	virtual void prepareRenderState() {}

	virtual void prepareConstantBuffer() {}

	virtual void prepareResources() {}

	/// <summary>
	/// 执行命令
	/// </summary>
	virtual void execute() {}

	virtual void destroy() {}

	bool IsPrepared() { return m_isPrepared; }
protected:
	bool m_isPrepared;
	CommandBuffer m_cmdBuffer;
	ScriptableRenderContext* m_context;
	RenderData* m_renderData;
};

using PassList = std::vector<PassBase*>;