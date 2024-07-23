#pragma once
#include <Object.h>
#include <ComponentType.h>
enum ComponentLifeCycle
{
	ComponentLifeCycle_None,
	ComponentLifeCycle_Awaked,
	ComponentLifeCycle_Running,
	ComponentLifeCycle_Destroyed
};
enum ComponentSubLifeCycle {
	ComponentSubLifeCycle_None,
	ComponentSubLifeCycle_Enabled,
	ComponentSubLifeCycle_Start,
	ComponentSubLifeCycle_Disabled,
	ComponentSubLifeCycle_Updating
};
class ComponentStateMachine;

class Component :public Object
{
public:
	friend class ComponentStateMachine;
	friend class GameObject;
	Component() {}
	~Component() {}

	void Destroy()
	{
		m_isReadyDead = true;
	}
	virtual ComponentType GetComponentType() = 0;

	inline bool IsDead() { return m_isDead; }
	inline GameObject* GetGameObject() { return m_gameObjectPtr; }
	__GET_SET_PROPERTY__(Enable, bool, m_enabled)
protected:
	virtual void OnAwake() {}
	virtual void OnStart() {}
	virtual void OnUpdate() {}
	virtual void OnDestroy() {}
	virtual void OnEnable() {}
	virtual void OnDisable() {}
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
	void Tick()
	{
		switch (m_target->m_state)
		{
		case ComponentLifeCycle_None:
		{
			PreAwake();
			break;
		}
		case ComponentLifeCycle_Awaked:
		{
			Awake();
			break;
		}
		case ComponentLifeCycle_Running:
		{
			Running();
			break;
		}
		case ComponentLifeCycle_Destroyed:
		{
			Destroy();
			break;
		}
		default:
			break;
		}
	}

	void Setup(std::shared_ptr<Component> component)
	{
		m_target = component;
	}

	inline ComponentLifeCycle GetComponentLifeStyle() { return m_target->m_state; }
private:
	void PreAwake()
	{
		m_target->m_state = ComponentLifeCycle::ComponentLifeCycle_Awaked;
	}
	void Awake()
	{
		m_target->OnAwake();
		if (m_target->m_isReadyDead)
		{
			m_target->m_state = ComponentLifeCycle::ComponentLifeCycle_Destroyed;
		}
		if (m_target->m_enabled)
		{
			m_target->m_state = ComponentLifeCycle::ComponentLifeCycle_Running;
			m_target->m_subState = ComponentSubLifeCycle::ComponentSubLifeCycle_Enabled;
		}
	}

	void Running()
	{
		switch (m_target->m_subState)
		{
		case ComponentSubLifeCycle_Enabled:
		{
			Enable();
			break;
		}
		case ComponentSubLifeCycle_Start:
		{
			Start();
			break;
		}
		case ComponentSubLifeCycle_Updating:
		{
			Update();
			break;
		}
		case ComponentSubLifeCycle_Disabled:
			Disable();
			break;
		default:
			break;
		}
	}

	void Start() {
		m_target->OnStart();
		m_target->m_subState = ComponentSubLifeCycle::ComponentSubLifeCycle_Updating;
	}

	void Update()
	{
		if (!m_target->m_enabled || m_target->m_isReadyDead)
		{
			m_target->m_subState = ComponentSubLifeCycle::ComponentSubLifeCycle_Disabled;
		}
		else {
			m_target->OnUpdate();
		}
	}
	void Enable() {
		m_target->OnEnable();
		if (!m_target->m_inited)
		{
			m_target->m_inited = true;
			m_target->m_subState = ComponentSubLifeCycle::ComponentSubLifeCycle_Start;
		}
		else {
			m_target->m_subState = ComponentSubLifeCycle::ComponentSubLifeCycle_Updating;
		}
	}

	void Disable()
	{
		m_target->OnDisable();
		if (m_target->m_isReadyDead)
		{
			m_target->m_state = ComponentLifeCycle::ComponentLifeCycle_Destroyed;
		}
	}

	void Destroy()
	{
		m_target->OnDestroy();
		m_target->m_isDead = true;
		m_target = nullptr;
	}
	std::shared_ptr<Component> m_target;
};