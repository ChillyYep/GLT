#include "Camera.h"

void Camera::recomputeViewMatrix()
{
	auto goPtr = static_cast<GameObject*>(m_gameObjectPtr);
	m_viewMatrix = computeViewMatrix(goPtr->getTransform()->getRotation(), goPtr->getTransform()->getPosition());
}

glm::mat4x4 Camera::computeViewMatrix(glm::quat rotation, glm::vec3 position)
{
	glm::mat4 viewMatrix;
	// 正交矩阵的转置矩阵就是其逆矩阵
	// 旋转+平移还原相机矩阵
	viewMatrix = glm::mat4_cast(rotation) * glm::translate(glm::identity<glm::mat4>(), -position);
	//viewMatrix = glm::translate(glm::identity<glm::mat4>(), -position) * glm::mat4_cast(rotation);
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
