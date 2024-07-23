#pragma once
#include <CommonDefine.h>
class InstanceIDProducer
{
public:
	InstanceIDProducer() :m_autoInstanceId(0) {};
	~InstanceIDProducer() {};

	inline GLuint ProduceInstanceId() { return ++m_autoInstanceId; }

private:
	GLuint m_autoInstanceId;
};
