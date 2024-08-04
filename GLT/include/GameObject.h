#pragma once
#include <Object.h>
#include <Component.h>
#include <type_traits>
#include <Transform.h>

class GameObject :public Object
{
public:
	GameObject() {}
	~GameObject() {}

	inline std::shared_ptr<Transform>& getTransform() { return m_transform; }

	void addComponent(std::shared_ptr<Component> component);

	template<typename T>
	std::shared_ptr<T> addComponent()
	{
		static_assert(TypeCheck::IsComponentType<T>(), "组件必须添加component_traits类型萃取");
		bool existSameType = false;
		for (int i = 0;i < m_components.size();++i)
		{
			if (m_components[i]->getComponentType() == component_traits<T>::value)
			{
				existSameType = true;
				break;
			}
		}
		assert(!existSameType);
		auto component = std::shared_ptr<Component>(new T());
		component->m_gameObjectPtr = this;
		m_components.push_back(component);
		if (component_traits<T>::value == ComponentType::Transform)
		{
			m_transform = std::static_pointer_cast<Transform>(component);
		}
		return std::static_pointer_cast<T>(component);
	}

	template<typename T>
	std::shared_ptr<T> getComponent()
	{
		if (!TypeCheck::IsComponentType<T>())
		{
			return nullptr;
		}
		for (int i = 0;i < m_components.size();++i)
		{
			if (m_components[i]->getComponentType() == component_traits<T>::value)
			{
				return std::static_pointer_cast<T>(m_components[i]);
			}
		}
		return nullptr;
	}

	void removeComponent(std::shared_ptr<Component> component);

	inline std::vector<std::shared_ptr<Component>> getComponents() { return m_components; }

	void destroy()
	{
		for (int i = 0;i < m_components.size();++i)
		{
			m_components[i]->destroy();
		}
	}

private:
	int getComponentIndex(std::shared_ptr<Component> component)
	{
		if (component == nullptr)
		{
			return -1;
		}
		for (int i = 0;i < m_components.size();++i)
		{
			if (m_components[i]->getInstanceId() == component->getInstanceId())
			{
				return i;
			}
		}
		return -1;
	}

	std::shared_ptr<Transform> m_transform;

	std::vector<std::shared_ptr<Component>> m_components;
};