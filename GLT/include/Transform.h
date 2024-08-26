#pragma once
#include <CommonDefine.h>
#include <Component.h>
#include <GLMath.h>

COMPONENT_CLASS(Transform, ComponentType::Transform)
{
public:
	Transform() :m_position(glm::zero<glm::vec4>()), m_rotation(glm::identity<glm::quat>()), m_scale(glm::one<glm::vec3>()) {};
	~Transform() {};

	ComponentType getComponentType() override { return ComponentType::Transform; }

	inline glm::mat4x4 getMatrix() {
		auto mat = glm::scale(glm::identity<glm::mat4x4>(), m_scale);
		mat = glm::mat4_cast(m_rotation) * mat;
		return glm::translate(mat, m_position);
	}
	inline void setMatrix(glm::mat4x4 modelMatrix)
	{
		glm::vec4 perspective;
		glm::vec3 skew;
		glm::decompose(modelMatrix, m_scale, m_rotation, m_position, skew, perspective);
	}
	void lookAt(glm::vec3 target);

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
	inline glm::vec3 getEularAngle() { return glm::degrees(glm::eulerAngles(m_rotation)); }

	inline void setEularAngle(glm::vec3 eularAngle) {
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
