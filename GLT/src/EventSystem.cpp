#include "EventSystem.h"
bool EventSystem::m_pressed;

MouseEvent EventSystem::m_uniqueMouseEvent;

WheelEvent EventSystem::m_uniqueWheelEvent;

GLFWwindow* EventSystem::m_currentWindow;

std::list<EventListener*> EventSystem::m_listeners;

void EventSystem::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	m_uniqueMouseEvent.m_pos = glm::vec2(x, y);
	// Pressed
	if (!m_pressed && action == GLFW_PRESS)
	{
		m_pressed = true;
		m_uniqueMouseEvent.m_mouseType = (MouseButtonType)button;
		for (const auto& listener : m_listeners)
		{
			if (listener != nullptr)
			{
				listener->onPointerPressed(m_uniqueMouseEvent);
			}
		}
	}
	// Released
	else if (m_pressed &&
		m_uniqueMouseEvent.m_mouseType == (MouseButtonType)button && action == GLFW_RELEASE)
	{
		m_pressed = false;
		for (const auto& listener : m_listeners)
		{
			if (listener != nullptr)
			{
				listener->onPointerReleased(m_uniqueMouseEvent);
			}
		}
		m_uniqueMouseEvent.m_mouseType = MouseButtonType::None;
	}
}

void EventSystem::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	m_uniqueMouseEvent.m_pos = glm::vec2(xpos, ypos);
	// Moving
	if (m_pressed)
	{
		for (const auto& listener : m_listeners)
		{
			if (listener != nullptr)
			{
				listener->onPointerMoved(m_uniqueMouseEvent);
			}
		}
	}
}

void EventSystem::wheelCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_uniqueWheelEvent.m_xOffset = static_cast<float>(xoffset);
	m_uniqueWheelEvent.m_yOffset = static_cast<float>(yoffset);
	for (const auto& listener : m_listeners)
	{
		if (listener != nullptr)
		{
			listener->onWheelScroll(m_uniqueWheelEvent);
		}
	}
}
