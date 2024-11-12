#pragma once
#include <ScriptableRenderContext.h>
#include <RenderData.h>
enum class PassState
{
	None,
	Inited,
	Preparing,
	Prepared,
	Executing,
	Destroyed
};

class PassBase
{
public:
	PassBase() :m_context(nullptr), m_passState(PassState::None), m_cmdBuffer() {}

	virtual ~PassBase() {}

	/// <summary>
	/// 设置环境
	/// </summary>
	void setup(ScriptableRenderContext* context, RenderData* renderData) { m_context = context; m_renderData = renderData; }

	/// <summary>
	/// 执行命令
	/// </summary>
	virtual void execute()
	{
		switch (m_passState)
		{
		case PassState::None:
		{
			onDefine();
			m_passState = PassState::Inited;
			break;
		}
		case PassState::Inited:
		{
			onPrepare();
			m_passState = PassState::Preparing;
			break;
		}
		case PassState::Preparing:
		{
			if (isPrepared())
			{
				m_passState = PassState::Prepared;
			}
			break;
		}
		case PassState::Prepared:
		{
			if (isExecutable())
			{
				m_passState = PassState::Executing;
			}
			break;
		}
		case PassState::Executing:
		{
			onExecute();
			break;
		}
		case PassState::Destroyed:
		{
			onDestroy();
			break;
		}
		default:
			break;
		}
	}

	void destroy()
	{
		m_passState = PassState::Destroyed;
	}
	bool isInit() { return m_passState != PassState::None; }
protected:
	virtual bool isPrepared() { return true; }

	virtual bool isExecutable() { return true; }
	/// <summary>
	/// 定义阶段
	/// </summary>
	virtual void onDefine()
	{
	}

	/// <summary>
	/// 预备阶段
	/// </summary>
	virtual void onPrepare()
	{
	}
	/// <summary>
	/// 执行阶段
	/// </summary>
	virtual void onExecute()
	{

	}
	/// <summary>
	/// 销毁阶段
	/// </summary>
	virtual void onDestroy()
	{

	}

	CommandBuffer m_cmdBuffer;
	ScriptableRenderContext* m_context;
	RenderData* m_renderData;
	PassState m_passState;
};

using PassList = std::vector<PassBase*>;