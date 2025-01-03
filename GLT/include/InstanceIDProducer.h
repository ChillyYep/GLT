#pragma once
#include <CommonDefine.h>
class InstanceIDProducer
{
public:
	InstanceIDProducer() :m_autoInstanceId(0) {};
	~InstanceIDProducer() {};

	inline GLTUInt32 produceInstanceId() { return ++m_autoInstanceId; }

private:
	GLTUInt32 m_autoInstanceId;
};
