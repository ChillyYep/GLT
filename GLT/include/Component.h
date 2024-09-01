#pragma once
#include <ComponentType.h>
#include <Object.h>
#include <memory>
class GameObject;

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

class Component :public Object
{
public:
	friend class ComponentStateMachine;
	friend class GameObject;
	Component() :m_enabled(true), m_gameObjectPtr(nullptr), m_isReadyDead(false),
		m_isDead(false), m_inited(false), m_state(ComponentLifeCycle::None), m_subState(ComponentSubLifeCycle::None) {}
	~Component() {}

	inline void destroy() { m_isReadyDead = true; }
	
	virtual ComponentType getComponentType() = 0;
	inline bool isDead() { return m_isDead; }
	inline GameObject* getGameObject() { return m_gameObjectPtr; }

	__GET_SET_PROPERTY__(Enable, bool, m_enabled)
protected:
	virtual void onAwake() {}
	virtual void onStart() {}
	virtual void onUpdate() {}
	virtual void onDestroy() {}
	virtual void onEnable() {}
	virtual void onDisable() {}
	bool m_enabled = true;
	GameObject* m_gameObjectPtr;
private:
	bool m_isReadyDead;
	bool m_isDead;
	bool m_inited;
	ComponentLifeCycle m_state;
	ComponentSubLifeCycle m_subState;
};
class ComponentStateMachine
{
public:
	void tick();
	inline void setup(Component* component){m_target = component;}
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