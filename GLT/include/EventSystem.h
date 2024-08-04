#pragma once
#include <CommonDefine.h>
#include <GLCommon.h>
#include <GLMath.h>
#include <list>
enum MouseButtonType {
	None = -1,
	LeftButton = GLFW_MOUSE_BUTTON_LEFT,
	RightButton = GLFW_MOUSE_BUTTON_RIGHT,
	MiddleButton = GLFW_MOUSE_BUTTON_MIDDLE
};
struct MouseEvent
{
	MouseButtonType m_mouseType;
	glm::vec2 m_pos;
};
struct WheelEvent
{
	float m_xOffset;
	float m_yOffset;
};


class EventListener
{
public:
	EventListener() {}
	~EventListener() {}

	virtual void onPointerPressed(MouseEvent mouseEvent) {}
	virtual void onPointerMoved(MouseEvent mouseEvent) {}
	virtual void onPointerReleased(MouseEvent mouseEvent) {}

	virtual void onWheelScroll(WheelEvent mouseEvent) {}

private:

};


class EventSystem
{
public:
	EventSystem() {}
	~EventSystem() {}
	inline static void attach(GLFWwindow* window) {
		m_currentWindow = window;
		glfwSetMouseButtonCallback(m_currentWindow, mouseButtonCallback);
		glfwSetScrollCallback(m_currentWindow, wheelCallback);
		glfwSetCursorPosCallback(m_currentWindow, cursorCallback);
	}
	inline static void dettach()
	{
		glfwSetMouseButtonCallback(m_currentWindow, nullptr);
		glfwSetScrollCallback(m_currentWindow, nullptr);
		glfwSetCursorPosCallback(m_currentWindow, nullptr);
		m_currentWindow = nullptr;
	}
	inline static void addListener(EventListener* listener) { m_listeners.push_back(listener); }
	inline static void removeListener(EventListener* listener) { m_listeners.remove(listener); }
private:

	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void cursorCallback(GLFWwindow* window, double xpos, double ypos);

	static void wheelCallback(GLFWwindow* window, double xoffset, double yoffset);

	static bool m_pressed;

	static MouseEvent m_uniqueMouseEvent;

	static WheelEvent m_uniqueWheelEvent;

	static GLFWwindow* m_currentWindow;

	static std::list<EventListener*> m_listeners;
};

