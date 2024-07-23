#pragma once
#include <InstanceIDProducer.h>

static InstanceIDProducer s_instanceIdProducer;

class Object {
public:
	Object()
	{
		m_instanceId = s_instanceIdProducer.ProduceInstanceId();
	}
	inline GLuint GetInstanceId() const { return m_instanceId; }

protected:
	GLuint m_instanceId;

	std::string m_name;

};