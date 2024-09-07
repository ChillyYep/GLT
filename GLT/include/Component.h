#pragma once
#include <ComponentType.h>
#include <Object.h>
#include <memory>
#include <vector>

enum class ComponentLifeCycle
{
	None,
	Awaked,
	Running,
	Destroyed
};
enum class ComponentSubLifeCycle {
	None,
	Enabled,
	Start,
	Disabled,
	Updating
};
class ComponentStateMachine;
class ComponentOwner;

class Component :public Object
{
public:
	friend class ComponentStateMachine;
	Component() :m_enabled(true), m_gameObjectPtr(nullptr), m_isReadyDead(false),
		m_isDead(false), m_inited(false), m_state(ComponentLifeCycle::None), m_subState(ComponentSubLifeCycle::None) {}
	virtual ~Component() {}

	inline void destroy() { m_isReadyDead = true; }

	virtual ComponentType getComponentType() = 0;
	inline bool isDead() { return m_isDead; }
	inline ComponentOwner* getGameObject() { return m_gameObjectPtr; }
	inline void setGameObject(ComponentOwner* owner) { m_gameObjectPtr = owner; }

	__GET_SET_PROPERTY__(Enable, bool, m_enabled)
protected:
	virtual void onAwake() {}
	virtual void onStart() {}
	virtual void onUpdate() {}
	virtual void onDestroy() {}
	virtual void onEnable() {}
	virtual void onDisable() {}
	bool m_enabled = true;
	ComponentOwner* m_gameObjectPtr;
private:
	bool m_isReadyDead;
	bool m_isDead;
	bool m_inited;
	ComponentLifeCycle m_state;
	ComponentSubLifeCycle m_subState;
};

class ComponentOwner :public Object
{
public:
	inline std::vector<std::shared_ptr<Component>> getComponents() { return m_components; }

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
		component->setGameObject(this);
		m_components.push_back(component);
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

	void addComponent(std::shared_ptr<Component> component);

	void removeComponent(std::shared_ptr<Component> component);

	virtual void destroy();

	__GET_SET_BOOLEANPROPERTY__(Destroyed, m_isdestroyed)
		__GET_SET_PROPERTY__(Actived, bool, m_actived)
private:
	int getComponentIndex(std::shared_ptr<Component> component);
protected:
	std::vector<std::shared_ptr<Component>> m_components;
	bool m_isdestroyed;

	bool m_actived = true;


};

class ComponentStateMachine
{
public:
	void tick();
	inline void setup(Component* component) { m_target = component; }
	inline ComponentLifeCycle getComponentLifeStyle() { return m_target->m_state; }
private:
	void preAwake();
	void awake();
	void running();
	void start();
	void update();
	void enable();
	void disable();
	void destroy();
	Component* m_target;
};