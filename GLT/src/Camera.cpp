#include "Camera.h"

void Camera::recomputeViewMatrix()
{
	auto goPtr = static_cast<GameObject*>(m_gameObjectPtr);
	m_viewMatrix = computeViewMatrix(goPtr->getTransform()->getRotation(), goPtr->getTransform()->getPosition());
}

glm::mat4x4 Camera::computeViewMatrix(glm::quat rotation, glm::vec3 position)
{
	glm::mat4 viewMatrix;
	// 旋转+平移还原相机矩阵
	viewMatrix = glm::mat4_cast(rotation) * glm::translate(glm::identity<glm::mat4>(), -position);
	return viewMatrix;
}
glm::mat4x4 Camera::computeOrthoProjectionMatrix(float aspect, float height, float nearPlane, float farPlane)
{
	glm::mat4x4 projectMatrix;
	GLfloat width = aspect * height;
	projectMatrix = glm::ortho(-width, width, -height, height, nearPlane, farPlane);
	return projectMatrix;
}

glm::mat4x4 Camera::computePerspectiveProjectionMatrix(float aspect, float fov, float nearPlane, float farPlane)
{
	glm::mat4x4 projectMatrix;
	projectMatrix = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
	return projectMatrix;
}

void Camera::recomputeProjectMatrix()
{
	if (m_ortho)
	{
		m_projectMatrix = computeOrthoProjectionMatrix(m_aspect, m_height, m_nearFar.x, m_nearFar.y);
	}
	else {
		m_projectMatrix = computePerspectiveProjectionMatrix(m_aspect, m_fov, m_nearFar.x, m_nearFar.y);
	}
}
