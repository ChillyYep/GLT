#include "Transform.h"

void Transform::lookAt(glm::vec3 target)
{
	m_rotation = glm::quatLookAt(target - m_position, glm::vec3(0, 1, 0));
	//glm::vec3 forward = getForward();
	//auto targetForward = glm::normalize(m_position - target);
	//auto axis = glm::cross(forward, targetForward);
	//auto angle = glm::acos(glm::dot(forward, targetForward));
	//m_rotation = glm::rotate(m_rotation, angle, axis);
}