#pragma once
class PassBase
{
public:
	PassBase() {}
	~PassBase() {}
	/// <summary>
	/// ����
	/// </summary>
	virtual void compile() = 0;
	/// <summary>
	/// ���ñ�����ִ�л���
	/// </summary>
	virtual void execute() = 0;
private:

};
