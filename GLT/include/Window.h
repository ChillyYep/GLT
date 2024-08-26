#pragma once
#include <Singleton.h>
#include <CommonDefine.h>
#include <GLMath.h>
#include <GLCommon.h>
#include <GLFWWindow.h>
class Window :public Singleton<Window>
{
public:
	~Window()
	{
		if (m_windowImplent != nullptr)
		{
			delete m_windowImplent;
			m_windowImplent = nullptr;
		}
	}

	void init()
	{
		m_windowImplent = new GlFWWindowImplement();
		m_windowImplent->init();
	}
	void uninit()
	{
		m_windowImplent->uninit();
	}
	void create(int width, int height, const char* title)
	{
		m_windowImplent->create(width, height, title);
	}
	void destroy()
	{
		m_windowImplent->destroy();
	}

	void setSize(int width, int height)
	{
		m_windowImplent->setSize(width, height);
	}

	glm::ivec2 getSize()
	{
		return m_windowImplent->getSize();
	}

	void attachToEventSystem()
	{
		m_windowImplent->attachToEventSystem();
	}
	void detachToEventSystem()
	{
		m_windowImplent->detachToEventSystem();
	}

	void gameLoop(WindowImplementBase::RenderLoopMainFunc renderLoopMainFunc)
	{
		m_windowImplent->gameLoop(renderLoopMainFunc);
	}
private:
	WindowImplementBase* m_windowImplent;
};