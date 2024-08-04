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
		// ��ʼ��GLFW��
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
			// GLFW����ֹ
			glfwTerminate();
			s_inited = false;
		}
	}

	void Create(int width, int height, const char* title) override
	{
		// ʹ�ú���ģʽ�����趨�ð汾
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		// ͨ��GLFW��������
		m_windowPtr = glfwCreateWindow(width, height, title, NULL, NULL);
		assert(m_windowPtr != nullptr);
		glfwMakeContextCurrent(m_windowPtr);
	}
	void Destroy()
	{
		if (m_windowPtr != nullptr)
		{
			// GLFW�����ٴ���
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
		// GLFW������Ⱦѭ��
		while (!glfwWindowShouldClose(m_windowPtr))
		{
			// ���ƽ׶�
			renderLoopMainFunc();
			glfwSwapBuffers(m_windowPtr);
			glfwPollEvents();
		}
	}
private:
	static bool s_inited;
	GLFWwindow* m_windowPtr;
};