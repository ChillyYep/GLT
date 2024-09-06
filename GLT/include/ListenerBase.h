#pragma once

class ListenerBase
{
public:
	ListenerBase() {}
	~ListenerBase() {}
	virtual void onNotify(int eventId) = 0;
private:
};