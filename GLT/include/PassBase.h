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
	virtual void prepare()
	{
		m_isPrepared = true;
	}

	/// <summary>
	/// 执行命令
	/// </summary>
	virtual void execute() {}

	virtual void destroy() {}

	bool isPrepared() { return m_isPrepared; }

	virtual bool isExecutable() { return true; }
protected:
	bool m_isPrepared;
	CommandBuffer m_cmdBuffer;
	ScriptableRenderContext* m_context;
	RenderData* m_renderData;
};

using PassList = std::vector<PassBase*>;