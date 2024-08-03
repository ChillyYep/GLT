#pragma once
#include <Singleton.h>
#include <CommonDefine.h>
#include <GLFW/glfw3.h>
#include <glfwWindow.h>
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

	void Init()
	{
		m_windowImplent = new GlFWWindowImplement();
		m_windowImplent->Init();
	}
	void UnInit()
	{
		m_windowImplent->UnInit();
	}
	void Create(int width, int height, const char* title)
	{
		m_windowImplent->Create(width, height, title);
	}
	void Destroy()
	{
		m_windowImplent->Destroy();
	}

	void SetSize(int width, int height)
	{
		m_windowImplent->SetSize(width, height);
	}

	glm::ivec2 GetSize()
	{
		return m_windowImplent->GetSize();
	}

	void AttachToEventSystem()
	{
		m_windowImplent->AttachToEventSystem();
	}
	void DetachToEventSystem()
	{
		m_windowImplent->DetachToEventSystem();
	}

	void GameLoop(WindowImplementBase::RenderLoopMainFunc renderLoopMainFunc)
	{
		m_windowImplent->GameLoop(renderLoopMainFunc);
	}
private:
	WindowImplementBase* m_windowImplent;
};