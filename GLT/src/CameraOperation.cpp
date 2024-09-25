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
			auto newRotation = rotate(m_beginRotation, offset.x, offset.y);
			m_transform->setRotation(newRotation);
		}
	}
	else if (mouseEvent.m_mouseType == MouseButtonType::MiddleButton)
	{
		if (m_transform != nullptr)
		{
			auto offsetXY = (mouseEvent.m_pos - m_beginMovePos) * m_translateSensitivity;
			auto newPosition = translate(m_beginCameraPos, offsetXY.x, offsetXY.y);
			m_transform->setPosition(newPosition);
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
			auto newRotation = rotate(m_beginRotation, offset.x, offset.y);
			m_transform->setRotation(newRotation);
		}
		m_beginRotatePos = glm::vec2(0.0f);
		m_beginRotation = glm::identity<glm::quat>();
	}
	else if (mouseEvent.m_mouseType == MouseButtonType::MiddleButton)
	{
		if (m_transform != nullptr)
		{
			auto offsetXY = (mouseEvent.m_pos - m_beginMovePos) * m_translateSensitivity;
			auto newPosition = translate(m_beginCameraPos, offsetXY.x, offsetXY.y);
			m_transform->setPosition(newPosition);
		}
		m_beginMovePos = glm::vec2(0.0f);
		m_beginCameraPos = glm::vec3(0.0f);
	}
}

void CameraController::onWheelScroll(WheelEvent wheelEvent) {
	auto newPosition = translate(m_transform->getPosition(), .0f, .0f, -wheelEvent.m_yOffset * m_zoomSensitivity);
	m_transform->setPosition(newPosition);
}

glm::quat CameraController::rotate(glm::quat beginQuat, float offsetX, float offsetY)
{
	auto rotateQuat = glm::quat_cast(glm::eulerAngleXYZ(glm::radians(offsetY), glm::radians(offsetX), 0.0f));
	beginQuat = rotateQuat * beginQuat;
	return beginQuat;
}

glm::vec3 CameraController::translate(glm::vec3 position, float offsetX, float offsetY, float offsetZ)
{
	auto offset = glm::vec3(-offsetX, offsetY, offsetZ);
	auto view2World = glm::inverse(m_transform->getRotation());
	offset = glm::rotate(view2World, offset);
	return position + offset;
}