#pragma once
#include <Object.h>
#include <Component.h>
#include <type_traits>

class GameObject :public Object
{
public:
	GameObject() {}
	~GameObject() {}

	inline std::shared_ptr<Transform>& GetTransform()
	{
		return m_transform;
	}

	void AddComponent(std::shared_ptr<Component> component)
	{
		assert(GetComponentIndex(component) < 0);
		component->m_gameObjectPtr = this;
		m_components.push_back(component);
	}

	template<typename T>
	std::shared_ptr<T> AddComponent()
	{
		static_assert(TypeCheck::IsComponentType<T>(), "组件必须添加component_traits类型萃取");
		bool existSameType = false;
		for (int i = 0;i < m_components.size();++i)
		{
			if (m_components[i]->GetComponentType() == component_traits<T>::value)
			{
				existSameType = true;
				break;
			}
		}
		assert(!existSameType);
		auto component = std::shared_ptr<Component>(new T());
		component->m_gameObjectPtr = this;
		m_components.push_back(component);
		if (component_traits<T>::value == ComponentType::ComponentType_Transform)
		{
			m_transform = std::static_pointer_cast<Transform>(component);
		}
		return std::static_pointer_cast<T>(component);
	}

	void RemoveComponent(std::shared_ptr<Component> component)
	{
		auto targetComponentIndex = GetComponentIndex(component);
		if (targetComponentIndex >= 0)
		{
			m_components[targetComponentIndex]->m_gameObjectPtr = nullptr;
			m_components[targetComponentIndex] = nullptr;
			for (int i = targetComponentIndex;i < m_components.size() - 1;++i)
			{
				m_components[i] = m_components[i + 1];
			}
			m_components.pop_back();
		}
	}

	inline std::vector<std::shared_ptr<Component>> GetComponents() { return m_components; }

	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		if (!TypeCheck::IsComponentType<T>())
		{
			return nullptr;
		}
		for (int i = 0;i < m_components.size();++i)
		{
			if (m_components[i]->GetComponentType() == component_traits<T>::value)
			{
				return std::static_pointer_cast<T>(m_components[i]);
			}
		}
		return nullptr;
	}

	void Destroy()
	{
		for (int i = 0;i < m_components.size();++i)
		{
			m_components[i]->Destroy();
		}
	}

private:
	int GetComponentIndex(std::shared_ptr<Component> component)
	{
		if (component == nullptr)
		{
			return -1;
		}
		for (int i = 0;i < m_components.size();++i)
		{
			if (m_components[i]->GetInstanceId() == component->GetInstanceId())
			{
				return i;
			}
		}
		return -1;
	}
	std::shared_ptr<Transform> m_transform;

	std::vector<std::shared_ptr<Component>> m_components;
};