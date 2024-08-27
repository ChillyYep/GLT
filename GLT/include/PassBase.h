#pragma once
#include <Shader.h>

class PassBase
{
public:
	PassBase() {}
	virtual ~PassBase() {}

	/// <summary>
	/// 设置环境
	/// </summary>
	void setUp(){}

	/// <summary>
	/// 预备
	/// </summary>
	virtual void prepare(){}

	/// <summary>
	/// 执行命令
	/// </summary>
	virtual void execute(){}
private:
	Shader* m_shaderPtr;
};

using PassList = std::vector<PassBase*>;