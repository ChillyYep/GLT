#pragma once
#include <CommonDefine.h>
#include <GLMath.h>
class WindowImplementBase
{
public:
	typedef void(*RenderLoopMainFunc)(void);
	WindowImplementBase() {}
	~WindowImplementBase() {}

	virtual void Init() = 0;
	virtual void UnInit() = 0;
	virtual void Create(int width, int height, const char* title) = 0;
	virtual void Destroy() = 0;

	virtual void SetSize(int width, int height) = 0;

	virtual glm::ivec2 GetSize() = 0;

	virtual void AttachToEventSystem() = 0;
	virtual void DetachToEventSystem() = 0;
	virtual void GameLoop(RenderLoopMainFunc renderLoopMainFunc) = 0;
};