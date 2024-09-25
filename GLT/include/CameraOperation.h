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

	inline void attach(std::shared_ptr<Transform>& transform)
	{
		m_transform = transform;
		EventSystem::addListener(this);
	}
	inline void dettach()
	{
		EventSystem::removeListener(this);
		m_transform = nullptr;
	}
	void onPointerPressed(MouseEvent mouseEvent) override;

	void onPointerMoved(MouseEvent mouseEvent) override;

	void onPointerReleased(MouseEvent mouseEvent) override;

	void onWheelScroll(WheelEvent wheelEvent) override;

	glm::quat rotate(glm::quat beginQuat, float offsetX, float offsetY);

	glm::vec3 translate(glm::vec3 position, float offsetX, float offsetY, float offsetZ = 0.0f);
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
