#pragma once
#include <ListenerBase.h>
#include <unordered_map>
#include <vector>
#include <assert.h>
class EventObserver
{
public:
	EventObserver() {}
	~EventObserver() {}

	void addListener(int eventId, ListenerBase* listener)
	{
		if (m_eventMap.find(eventId) == m_eventMap.end())
		{
			m_eventMap[eventId] = std::vector<ListenerBase*>();
		}
		m_eventMap[eventId].push_back(listener);
	}

	void removeListener(int eventId, ListenerBase* listener)
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

	void trigger(int eventId)
	{
		if (m_eventMap.find(eventId) == m_eventMap.end())
		{
			return;
		}
		for (const auto& listener : m_eventMap[eventId])
		{
			listener->onNotify(eventId);
		}
	}

private:
	std::unordered_map<int, std::vector<ListenerBase*>> m_eventMap;
};
