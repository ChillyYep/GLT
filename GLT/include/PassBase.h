#pragma once
class PassBase
{
public:
	PassBase() {}
	~PassBase() {}
	/// <summary>
	/// ����
	/// </summary>
	virtual void Compile() = 0;
	/// <summary>
	/// ���ñ�����ִ�л���
	/// </summary>
	virtual void Execute() = 0;
private:

};
