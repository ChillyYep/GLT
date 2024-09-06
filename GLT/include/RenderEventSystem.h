#pragma once
#include <ListenerBase.h>
#include <unordered_map>
#include <vector>
#include <assert.h>
#include <Singleton.h>
#include <RenderEventId.h>

class RenderEvent
{
public:
	RenderEventId m_eventId;
	void* m_param;
};

class RenderEventListener
{
public:
	RenderEventListener() {}
	virtual ~RenderEventListener() {}
	virtual void onNotify(RenderEvent& renderEvent) = 0;
};

class RenderEventSystem :public Singleton<RenderEventSystem>
{
public:
	~RenderEventSystem() {}

	void addListener(RenderEventId eventId, RenderEventListener* listener)
	{
		if (m_eventMap.find(eventId) == m_eventMap.end())
		{
			m_eventMap[eventId] = std::vector<RenderEventListener*>();
		}
		m_eventMap[eventId].push_back(listener);
	}

	void removeListener(RenderEventId eventId, RenderEventListener* listener)
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

	void notify(RenderEvent& renderEvent)
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
private:
	std::unordered_map<RenderEventId, std::vector<RenderEventListener*>> m_eventMap;
};

class RenderEventFactory:public Singleton<RenderEventFactory>
{
public:
	~RenderEventFactory(){}

private:

};