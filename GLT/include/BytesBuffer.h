#pragma once
#include <CommonDefine.h>
#include <vcruntime_string.h>

class BytesBuffer
{
public:
	BytesBuffer() :m_dataPtr(nullptr), m_dataSize(0) {}
	~BytesBuffer() {}

	void create(GLTUInt32 bufferSize)
	{
		m_dataSize = bufferSize;
		m_dataPtr = new GLTUByte[bufferSize];
		for (GLTUInt32 i = 0;i < bufferSize;++i)
		{
			((GLTUByte*)m_dataPtr)[i] = 0xff;
		}
	}

	void release()
	{
		if (m_dataPtr != nullptr)
		{
			delete[] m_dataPtr;
			m_dataPtr = nullptr;
		}
	}

	void setData(GLTUInt32 offset, GLTUInt32 length, void* data)
	{
		if ((offset + length) <= m_dataSize)
		{
			GLTUByte* offsetDataPtr = reinterpret_cast<GLTUByte*>(m_dataPtr) + offset;
			memcpy(offsetDataPtr, data, length);
		}
	}

	inline const void* data() { return m_dataPtr; }
	inline GLTSize dataSize() { return m_dataSize; }
private:
	void* m_dataPtr;

	GLTSize m_dataSize;
};
