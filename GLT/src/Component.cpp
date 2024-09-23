#include "Component.h"

void ComponentStateMachine::tick()
{
	switch (m_target->m_state)
	{
	case ComponentLifeCycle::None:
	{
		preAwake();
		break;
	}
	case ComponentLifeCycle::Awaked:
	{
		awake();
		break;
	}
	case ComponentLifeCycle::Running:
	{
		running();
		break;
	}
	case ComponentLifeCycle::Destroyed:
	{
		destroy();
		break;
	}
	default:
		break;
	}
}

void ComponentStateMachine::awake()
{
	m_target->onAwake();
	if (m_target->m_isReadyDead)
	{
		m_target->m_state = ComponentLifeCycle::Destroyed;
	}
	if (m_target->m_enabled)
	{
		m_target->m_state = ComponentLifeCycle::Running;
		m_target->m_subState = ComponentSubLifeCycle::Enabled;
	}
}

void ComponentStateMachine::running()
{
	switch (m_target->m_subState)
	{
	case ComponentSubLifeCycle::Enabled:
	{
		enable();
		break;
	}
	case ComponentSubLifeCycle::Start:
	{
		start();
		break;
	}
	case ComponentSubLifeCycle::Updating:
	{
		update();
		break;
	}
	case ComponentSubLifeCycle::Disabled:
		disable();
		break;
	default:
		break;
	}
}

void ComponentStateMachine::update()
{
	if (!m_target->m_enabled || m_target->m_isReadyDead)
	{
		m_target->m_subState = ComponentSubLifeCycle::Disabled;
	}
	else {
		m_target->onUpdate();
	}
}
void ComponentStateMachine::start() {
	m_target->onStart();
	m_target->m_subState = ComponentSubLifeCycle::Updating;
}

void ComponentStateMachine::preAwake()
{
	m_target->m_state = ComponentLifeCycle::Awaked;
}

void ComponentStateMachine::enable() {
	m_target->onEnable();
	if (!m_target->m_inited)
	{
		m_target->m_inited = true;
		m_target->m_subState = ComponentSubLifeCycle::Start;
	}
	else {
		m_target->m_subState = ComponentSubLifeCycle::Updating;
	}
}

void ComponentStateMachine::disable()
{
	m_target->onDisable();
	if (m_target->m_isReadyDead)
	{
		m_target->m_state = ComponentLifeCycle::Destroyed;
	}
}

void ComponentStateMachine::destroy()
{
	m_target->onDestroy();
	m_target->m_isDead = true;
	m_target = nullptr;
}


void ComponentOwner::addComponent(std::shared_ptr<Component> component)
{
	assert(getComponentIndex(component) < 0);
	component->setGameObject(this);
	m_components.push_back(component);
}
void ComponentOwner::removeComponent(std::shared_ptr<Component> component)
{
	auto targetComponentIndex = getComponentIndex(component);
	if (targetComponentIndex >= 0)
	{
		m_components[targetComponentIndex]->setGameObject(nullptr);
		m_components[targetComponentIndex] = nullptr;
		for (int i = targetComponentIndex;i < m_components.size() - 1;++i)
		{
			m_components[i] = m_components[i + 1];
		}
		m_components.pop_back();
	}
}

int ComponentOwner::getComponentIndex(std::shared_ptr<Component> component)
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

void ComponentOwner::destroy()
{
	if (m_isdestroyed)
	{
		return;
	}
	m_isdestroyed = true;
	// ×é¼þÏú»Ù
	for (int i = 0;i < m_components.size();++i)
	{
		m_components[i]->destroy();
	}
	m_components.clear();
}