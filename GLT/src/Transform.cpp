#include "Transform.h"

void Transform::lookAt(glm::vec3 target)
{
	glm::vec3 forward = Forward();
	auto targetForward = glm::normalize(m_position - target);
	auto axis = glm::cross(forward, targetForward);
	auto angle = glm::acos(glm::dot(forward, targetForward));
	m_rotation = glm::rotate(m_rotation, angle, axis);
}