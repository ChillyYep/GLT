#include "CameraOperation.h"

void CameraController::onPointerPressed(MouseEvent mouseEvent)
{
	if (mouseEvent.m_mouseType == MouseButtonType::LeftButton)
	{

	}
	else if (mouseEvent.m_mouseType == MouseButtonType::RightButton)
	{
		m_beginRotatePos = mouseEvent.m_pos;
		if (m_transform != nullptr)
		{
			m_beginRotation = m_transform->getRotation();
		}
	}
	else if (mouseEvent.m_mouseType == MouseButtonType::MiddleButton)
	{
		m_beginMovePos = mouseEvent.m_pos;
		if (m_transform != nullptr)
		{
			m_beginCameraPos = m_transform->getPosition();
		}
	}
}

void CameraController::onPointerMoved(MouseEvent mouseEvent)
{
	if (mouseEvent.m_mouseType == MouseButtonType::LeftButton)
	{

	}
	else if (mouseEvent.m_mouseType == MouseButtonType::RightButton)
	{
		auto offset = (mouseEvent.m_pos - m_beginRotatePos) * m_rotateSensitivity;
		if (m_transform != nullptr)
		{
			auto newRotation = rotate(m_beginRotation, -offset.y, -offset.x);
			m_transform->setRotation(newRotation);
		}
	}
	else if (mouseEvent.m_mouseType == MouseButtonType::MiddleButton)
	{
		if (m_transform != nullptr)
		{
			auto right = m_transform->Right();
			auto up = m_transform->Up();
			auto offset = (mouseEvent.m_pos - m_beginMovePos) * m_translateSensitivity;
			m_transform->setPosition(m_beginCameraPos + right * -offset.x + up * offset.y);
		}
	}
}

void CameraController::onPointerReleased(MouseEvent mouseEvent)
{
	if (mouseEvent.m_mouseType == MouseButtonType::LeftButton)
	{

	}
	else if (mouseEvent.m_mouseType == MouseButtonType::RightButton)
	{
		auto offset = (mouseEvent.m_pos - m_beginRotatePos) * m_rotateSensitivity;
		if (m_transform != nullptr)
		{
			auto newRotation = rotate(m_beginRotation, -offset.y, -offset.x);
			m_transform->setRotation(newRotation);
		}
		m_beginRotatePos = glm::vec2(0.0f);
		m_beginRotation = glm::identity<glm::quat>();
	}
	else if (mouseEvent.m_mouseType == MouseButtonType::MiddleButton)
	{
		if (m_transform != nullptr)
		{
			auto right = m_transform->Right();
			auto up = m_transform->Up();
			auto offset = (mouseEvent.m_pos - m_beginMovePos) * m_translateSensitivity;
			m_transform->setPosition(m_beginCameraPos + right * -offset.x + up * offset.y);
		}
		m_beginMovePos = glm::vec2(0.0f);
		m_beginCameraPos = glm::vec3(0.0f);
	}
}

void CameraController::onWheelScroll(WheelEvent wheelEvent) {
	auto forward = m_transform->Forward();
	m_transform->Translate(forward * -wheelEvent.m_yOffset * m_zoomSensitivity);
}

glm::quat CameraController::rotate(glm::quat beginQuat, float offsetY, float offsetX)
{
	auto rotateQuat = glm::quat_cast(glm::eulerAngleXYZ(glm::radians(offsetY), glm::radians(offsetX), 0.0f));
	beginQuat = beginQuat * rotateQuat;
	return beginQuat;
}