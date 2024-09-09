#include "Camera.h"

void Camera::recomputeViewMatrix()
{
	auto goPtr = static_cast<GameObject*>(m_gameObjectPtr);
	m_viewMatrix = computeViewMatrix(goPtr->getTransform()->getRotation(), goPtr->getTransform()->getPosition());
}

glm::mat4x4 Camera::computeViewMatrix(glm::quat rotation, glm::vec3 position)
{
	glm::mat4 viewMatrix;
	glm::vec3 left = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	left = rotation * left;
	up = rotation * up;
	forward = rotation * forward;
	// 正交矩阵的转置矩阵就是其逆矩阵
	// 旋转+平移还原视图矩阵
	viewMatrix = glm::transpose(glm::mat4(glm::vec4(left, 0.0f), glm::vec4(up, 0.0f), glm::vec4(forward, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0)));
	viewMatrix = glm::translate(viewMatrix, -position);
	return viewMatrix;
}

void Camera::recomputeProjectMatrix()
{
	if (m_ortho)
	{
		GLfloat width = m_aspect * m_height;
		m_projectMatrix = glm::ortho(-width, width, -m_height, m_height, m_nearFar.x, m_nearFar.y);
	}
	else {
		m_projectMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_nearFar.x, m_nearFar.y);
	}
}
