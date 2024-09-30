#pragma once
#include <string>
#include <InstanceIDProducer.h>

static InstanceIDProducer s_instanceIdProducer;

class Object {
public:
	Object()
	{
		m_instanceId = s_instanceIdProducer.produceInstanceId();
	}
	virtual ~Object() {}
	inline GLTUInt32 getInstanceId() const { return m_instanceId; }
	std::string m_name;
protected:
	GLTUInt32 m_instanceId;
};