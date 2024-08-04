#pragma once
class PassBase
{
public:
	PassBase() {}
	~PassBase() {}
	/// <summary>
	/// 编译
	/// </summary>
	virtual void compile() = 0;
	/// <summary>
	/// 设置变量，执行绘制
	/// </summary>
	virtual void execute() = 0;
private:

};
