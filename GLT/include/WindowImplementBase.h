#pragma once
#include <CommonDefine.h>
#include <GLMath.h>
class WindowImplementBase
{
public:
	typedef void(*RenderLoopMainFunc)(void);
	WindowImplementBase() {}
	~WindowImplementBase() {}

	virtual void init() = 0;
	virtual void uninit() = 0;
	virtual void create(int width, int height, const char* title) = 0;
	virtual void destroy() = 0;

	virtual void setSize(int width, int height) = 0;

	virtual glm::ivec2 getSize() = 0;

	virtual void attachToEventSystem() = 0;
	virtual void detachToEventSystem() = 0;
	virtual void gameLoop(RenderLoopMainFunc renderLoopMainFunc) = 0;
};