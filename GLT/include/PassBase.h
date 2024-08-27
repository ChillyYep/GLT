#pragma once
#include <Shader.h>

class PassBase
{
public:
	PassBase() {}
	virtual ~PassBase() {}

	/// <summary>
	/// ���û���
	/// </summary>
	void setUp(){}

	/// <summary>
	/// Ԥ��
	/// </summary>
	virtual void prepare(){}

	/// <summary>
	/// ִ������
	/// </summary>
	virtual void execute(){}
private:
	Shader* m_shaderPtr;
};

using PassList = std::vector<PassBase*>;