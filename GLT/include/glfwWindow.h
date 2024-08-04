#pragma once
#include <CommonDefine.h>
#include <WindowImplementBase.h>
#include <GLCommon.h>
#include <EventSystem.h>

class GlFWWindowImplement :public WindowImplementBase
{
public:
	GlFWWindowImplement() :WindowImplementBase() {}
	~GlFWWindowImplement() {}
	void Init() override
	{
		// 初始化GLFW库
		if (!s_inited)
		{
			assert(glfwInit());
			s_inited = true;
		}
	}
	void UnInit() override
	{
		if (s_inited)
		{
			// GLFW库终止
			glfwTerminate();
			s_inited = false;
		}
	}

	void Create(int width, int height, const char* title) override
	{
		// 使用核心模式，且设定好版本
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		// 通过GLFW创建窗口
		m_windowPtr = glfwCreateWindow(width, height, title, NULL, NULL);
		assert(m_windowPtr != nullptr);
		glfwMakeContextCurrent(m_windowPtr);
	}
	void Destroy()
	{
		if (m_windowPtr != nullptr)
		{
			// GLFW库销毁窗口
			glfwDestroyWindow(m_windowPtr);
			m_windowPtr = nullptr;
		}
	}
	void SetSize(int width, int height) override
	{
		glfwSetWindowSize(m_windowPtr, width, height);
	}

	glm::ivec2 GetSize() override
	{
		glm::ivec2 size;
		glfwGetWindowSize(m_windowPtr, &size.x, &size.y);
		return size;
	}

	void AttachToEventSystem() override
	{
		EventSystem::Attach(m_windowPtr);
	}
	void DetachToEventSystem() override
	{
		EventSystem::Dettach();
	}
	void GameLoop(RenderLoopMainFunc renderLoopMainFunc) override
	{
		// GLFW窗口渲染循环
		while (!glfwWindowShouldClose(m_windowPtr))
		{
			// 绘制阶段
			renderLoopMainFunc();
			glfwSwapBuffers(m_windowPtr);
			glfwPollEvents();
		}
	}
private:
	static bool s_inited;
	GLFWwindow* m_windowPtr;
};