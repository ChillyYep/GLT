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

	void addListener(RenderEventId eventId, RenderEventListener* listener);

	void removeListener(RenderEventId eventId, RenderEventListener* listener);

	void notify(RenderEvent& renderEvent);
private:
	std::unordered_map<RenderEventId, std::vector<RenderEventListener*>> m_eventMap;
};
