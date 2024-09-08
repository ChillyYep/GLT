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

	inline glm::mat4 getModelMatrix() {
		return glm::translate(m_position)* glm::mat4_cast(m_rotation)* glm::scale(m_scale);
	}

	inline void setModelMatrix(glm::mat4 modelMatrix)
	{
		glm::vec4 perspective;
		glm::vec3 skew;
		glm::decompose(modelMatrix, m_scale, m_rotation, m_position, skew, perspective);
	}

	void lookAt(glm::vec3 target);

	inline void translate(glm::vec3 offset) {
		m_position += offset;
	}
	inline glm::vec3 getForward()
	{
		return getForward(m_rotation);
	}

	inline glm::vec3 getRight()
	{
		return getRight(m_rotation);
	}

	inline glm::vec3 getUp()
	{
		return getUp(m_rotation);
	}

	static inline glm::vec3 getForward(glm::quat & quat) {
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
		forward = quat * forward;
		return forward;
	}
	static inline glm::vec3 getRight(glm::quat & quat) {
		glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
		right = quat * right;
		return right;
	}
	static inline glm::vec3 getUp(glm::quat & quat) {
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		up = quat * up;
		return up;
	}
	inline glm::vec3 getEularAngle() { return glm::degrees(glm::eulerAngles(m_rotation)); }

	inline void setEularAngle(glm::vec3 eularAngle) {
		eularAngle = glm::radians(eularAngle);
		m_rotation = glm::quat_cast(glm::eulerAngleXYZ(eularAngle.x, eularAngle.y, eularAngle.z));
	}

	void addChild(Transform * child)
	{
		if (child == nullptr)
		{
			return;
		}
		// 如果已经在子节点中，直接退出
		if (child->m_parent == this)
		{
			return;
		}

		// 如果已经存在父节点，需先从原父节点中删除
		if (child->m_parent != nullptr)
		{
			child->m_parent->removeChild(child);
		}
		m_children.push_back(child);
		child->m_parent = this;
	}

	void setParent(Transform * parent)
	{
		if (parent == nullptr)
		{
			if (m_parent != nullptr)
			{
				m_parent->removeChild(this);
				m_parent = nullptr;
				return;
			}
		}
		else if (m_parent == parent)
		{
			return;
		}
		parent->addChild(this);
	}
	Transform* getParent()
	{
		return m_parent;
	}
	__GET_SET_PROPERTY__(Position, glm::vec3, m_position)
		__GET_SET_PROPERTY__(Scale, glm::vec3, m_scale)
		__GET_SET_PROPERTY__(Rotation, glm::quat, m_rotation)
		__GET_SET_PROPERTY__(Children, std::vector<Transform*>, m_children)
private:
	void removeChild(Transform * child)
	{
		if (child == nullptr)
		{
			return;
		}
		for (int i = 0;i < m_children.size();++i)
		{
			if (child->getInstanceId() == m_children[i]->getInstanceId())
			{
				// 子节点的父节点置空
				child->m_parent = nullptr;
				for (int j = i;j < m_children.size() - 1;++j)
				{
					m_children[j] = m_children[j + 1];
				}
				// 并从本节点中删除该子节点
				m_children.pop_back();
				break;
			}
		}
	}
	std::vector<Transform*> m_children;
	Transform* m_parent;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::quat m_rotation;
};
