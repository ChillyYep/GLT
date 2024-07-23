#pragma once
#include <CommonDefine.h>
#include <GLFW/glfw3.h>
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

	virtual void OnPointerPressed(MouseEvent mouseEvent) {}
	virtual void OnPointerMoved(MouseEvent mouseEvent) {}
	virtual void OnPointerReleased(MouseEvent mouseEvent) {}

	virtual void OnWheelScroll(WheelEvent mouseEvent) {}

private:

};


class EventSystem
{
public:
	EventSystem() {}
	~EventSystem() {}
	inline static void Attach(GLFWwindow* window) {
		m_currentWindow = window;
		glfwSetMouseButtonCallback(m_currentWindow, MouseButtonCallback);
		glfwSetScrollCallback(m_currentWindow, WheelCallback);
		glfwSetCursorPosCallback(m_currentWindow, CursorCallback);
	}
	inline static void Dettach()
	{
		glfwSetMouseButtonCallback(m_currentWindow, nullptr);
		glfwSetScrollCallback(m_currentWindow, nullptr);
		glfwSetCursorPosCallback(m_currentWindow, nullptr);
		m_currentWindow = nullptr;
	}
	inline static void AddListener(EventListener* listener) { m_listeners.push_back(listener); }
	inline static void RemoveListener(EventListener* listener) { m_listeners.remove(listener); }
private:

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void CursorCallback(GLFWwindow* window, double xpos, double ypos);

	static void WheelCallback(GLFWwindow* window, double xoffset, double yoffset);

	static bool m_pressed;

	static MouseEvent m_uniqueMouseEvent;

	static WheelEvent m_uniqueWheelEvent;

	static GLFWwindow* m_currentWindow;

	static std::list<EventListener*> m_listeners;
};

