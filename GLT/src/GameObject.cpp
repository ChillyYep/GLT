#include "GameObject.h"

void GameObject::removeComponent(std::shared_ptr<Component> component)
{
	auto targetComponentIndex = getComponentIndex(component);
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

void GameObject::addComponent(std::shared_ptr<Component> component)
{
	assert(getComponentIndex(component) < 0);
	component->m_gameObjectPtr = this;
	m_components.push_back(component);
}