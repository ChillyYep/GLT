#pragma once
#include <CommonDefine.h>
#include <Component.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

COMPONENT_CLASS(Transform, ComponentType::ComponentType_Transform)
{
public:
	Transform() :m_position(glm::zero<glm::vec4>()), m_rotation(glm::identity<glm::quat>()), m_scale(glm::one<glm::vec3>()) {};
	~Transform() {};

	ComponentType GetComponentType() override { return ComponentType::ComponentType_Transform; }

	inline glm::mat4x4 GetMatrix() {
		auto mat = glm::scale(glm::identity<glm::mat4x4>(), m_scale);
		mat = glm::mat4_cast(m_rotation) * mat;
		return glm::translate(mat, m_position);
	}
	inline void SetMatrix(glm::mat4x4 modelMatrix)
	{
		glm::vec4 perspective;
		glm::vec3 skew;
		glm::decompose(modelMatrix, m_scale, m_rotation, m_position, skew, perspective);
	}
	inline void LookAt(glm::vec3 target)
	{
		glm::vec3 forward = Forward();
		auto targetForward = glm::normalize(m_position - target);
		auto axis = glm::cross(forward, targetForward);
		auto angle = glm::acos(glm::dot(forward, targetForward));
		m_rotation = glm::rotate(m_rotation, angle, axis);
	}
	inline void Translate(glm::vec3 offset) {
		m_position += offset;
	}
	inline glm::vec3 Forward()
	{
		return Forward(m_rotation);
	}

	inline glm::vec3 Right()
	{
		return Right(m_rotation);
	}

	inline glm::vec3 Up()
	{
		return Up(m_rotation);
	}

	static inline glm::vec3 Forward(glm::quat & quat) {
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
		forward = quat * forward;
		return forward;
	}
	static inline glm::vec3 Right(glm::quat & quat) {
		glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
		right = quat * right;
		return right;
	}
	static inline glm::vec3 Up(glm::quat & quat) {
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		up = quat * up;
		return up;
	}
	inline glm::vec3 GetEularAngle() { return glm::degrees(glm::eulerAngles(m_rotation)); }

	inline void SetEularAngle(glm::vec3 eularAngle) {
		eularAngle = glm::radians(eularAngle);
		m_rotation = glm::quat_cast(glm::eulerAngleXYZ(eularAngle.x, eularAngle.y, eularAngle.z));
	}

	__GET_SET_PROPERTY__(Position, glm::vec3, m_position)

		__GET_SET_PROPERTY__(Scale, glm::vec3, m_scale)
		__GET_SET_PROPERTY__(Rotation, glm::quat, m_rotation)
private:

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::quat m_rotation;
};
