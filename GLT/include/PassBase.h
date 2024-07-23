#pragma once
class PassBase
{
public:
	PassBase() {}
	~PassBase() {}
	/// <summary>
	/// 编译
	/// </summary>
	virtual void Compile() = 0;
	/// <summary>
	/// 设置变量，执行绘制
	/// </summary>
	virtual void Execute() = 0;
private:

};
