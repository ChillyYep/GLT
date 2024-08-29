#pragma once
#include <ScriptableRenderContext.h>

class PassBase
{
public:
	PassBase() :m_context(nullptr), m_cmdBuffer() {}
	virtual ~PassBase() {}

	/// <summary>
	/// ���û���
	/// </summary>
	void setup(ScriptableRenderContext* context) { m_context = context; }

	/// <summary>
	/// Ԥ��
	/// </summary>
	virtual void prepare() { m_isPrepared = true; }

	/// <summary>
	/// ִ������
	/// </summary>
	virtual void execute() {}

	bool IsPrepared() { return m_isPrepared; }
protected:
	bool m_isPrepared;
	CommandBuffer m_cmdBuffer;
	ScriptableRenderContext* m_context;
};

using PassList = std::vector<PassBase*>;