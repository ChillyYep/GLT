#pragma once
#include <string>
#include <InstanceIDProducer.h>

static InstanceIDProducer s_instanceIdProducer;

class Object {
public:
	Object()
	{
		m_instanceId = s_instanceIdProducer.ProduceInstanceId();
	}
	inline GLTUInt32 GetInstanceId() const { return m_instanceId; }

protected:
	GLTUInt32 m_instanceId;

	std::string m_name;

};