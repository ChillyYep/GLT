#include "RenderEventSystem.h"

void RenderEventSystem::addListener(RenderEventId eventId, RenderEventListener* listener)
{
	if (m_eventMap.find(eventId) == m_eventMap.end())
	{
		m_eventMap[eventId] = std::vector<RenderEventListener*>();
	}
	m_eventMap[eventId].push_back(listener);
}

void RenderEventSystem::removeListener(RenderEventId eventId, RenderEventListener* listener)
{
	if (m_eventMap.find(eventId) == m_eventMap.end())
	{
		return;
	}
	auto& listeners = m_eventMap[eventId];
	for (int i = 0; i < listeners.size(); ++i)
	{
		if (listeners[i] == listener)
		{
			for (int j = i; j < listeners.size() - 1; ++j)
			{
				listeners[j] = listeners[j + 1];
			}
			listeners.pop_back();
			break;
		}
	}
}

void RenderEventSystem::notify(RenderEvent& renderEvent)
{
	auto eventId = renderEvent.m_eventId;
	if (m_eventMap.find(eventId) == m_eventMap.end())
	{
		return;
	}
	for (const auto& listener : m_eventMap[eventId])
	{
		listener->onNotify(renderEvent);
	}
}