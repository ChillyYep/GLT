#pragma once
#include <Transform.h>
#include <EventSystem.h>

/// <summary>
/// 相机控制类
/// </summary>
class CameraController :EventListener
{
public:
	CameraController() {}
	~CameraController() {}

	inline void Attach(std::shared_ptr<Transform>& transform)
	{
		m_transform = transform;
		EventSystem::addListener(this);
	}
	inline void Dettach()
	{
		EventSystem::removeListener(this);
		m_transform = nullptr;
	}
	inline void onPointerPressed(MouseEvent mouseEvent) override
	{
		if (mouseEvent.m_mouseType == MouseButtonType::LeftButton)
		{

		}
		else if (mouseEvent.m_mouseType == MouseButtonType::RightButton)
		{
			m_beginRotatePos = mouseEvent.m_pos;
			if (m_transform != nullptr)
			{
				m_beginRotation = m_transform->GetRotation();
			}
		}
		else if (mouseEvent.m_mouseType == MouseButtonType::MiddleButton)
		{
			m_beginMovePos = mouseEvent.m_pos;
			if (m_transform != nullptr)
			{
				m_beginCameraPos = m_transform->GetPosition();
			}
		}
	}
	inline void onPointerMoved(MouseEvent mouseEvent) override
	{
		if (mouseEvent.m_mouseType == MouseButtonType::LeftButton)
		{

		}
		else if (mouseEvent.m_mouseType == MouseButtonType::RightButton)
		{
			auto offset = (mouseEvent.m_pos - m_beginRotatePos) * m_rotateSensitivity;
			if (m_transform != nullptr)
			{
				auto newRotation = Rotate(m_beginRotation, -offset.y, -offset.x);
				m_transform->SetRotation(newRotation);
			}
		}
		else if (mouseEvent.m_mouseType == MouseButtonType::MiddleButton)
		{
			if (m_transform != nullptr)
			{
				auto right = m_transform->Right();
				auto up = m_transform->Up();
				auto offset = (mouseEvent.m_pos - m_beginMovePos) * m_translateSensitivity;
				m_transform->SetPosition(m_beginCameraPos + right * -offset.x + up * offset.y);
			}
		}
	}

	inline void onPointerReleased(MouseEvent mouseEvent) override
	{
		if (mouseEvent.m_mouseType == MouseButtonType::LeftButton)
		{

		}
		else if (mouseEvent.m_mouseType == MouseButtonType::RightButton)
		{
			auto offset = (mouseEvent.m_pos - m_beginRotatePos) * m_rotateSensitivity;
			if (m_transform != nullptr)
			{
				auto newRotation = Rotate(m_beginRotation, -offset.y, -offset.x);
				m_transform->SetRotation(newRotation);
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
				m_transform->SetPosition(m_beginCameraPos + right * -offset.x + up * offset.y);
			}
			m_beginMovePos = glm::vec2(0.0f);
			m_beginCameraPos = glm::vec3(0.0f);
		}
	}

	inline void onWheelScroll(WheelEvent wheelEvent) override {
		auto forward = m_transform->Forward();
		m_transform->Translate(forward * -wheelEvent.m_yOffset * m_zoomSensitivity);
	}

	inline glm::quat Rotate(glm::quat beginQuat, float offsetY, float offsetX)
	{
		auto rotateQuat = glm::quat_cast(glm::eulerAngleXYZ(glm::radians(offsetY), glm::radians(offsetX), 0.0f));
		beginQuat = beginQuat * rotateQuat;
		return beginQuat;
	}
private:
	glm::vec2 m_beginRotatePos;
	glm::vec2 m_beginMovePos;

	float m_rotateSensitivity = 0.05f;

	float m_translateSensitivity = 0.01f;

	float m_zoomSensitivity = 0.1f;

	glm::quat m_beginRotation;

	glm::vec3 m_beginCameraPos;

	std::shared_ptr<Transform> m_transform;
};
