#pragma once
#include <CommonDefine.h>

class BytesBuffer
{
public:
	BytesBuffer() :m_dataPtr(nullptr) {}
	~BytesBuffer() {}

	void Create(GLuint bufferSize)
	{
		m_dataSize = bufferSize;
		m_dataPtr = new GLubyte[bufferSize];
		for (GLuint i = 0;i < bufferSize;++i)
		{
			((GLubyte*)m_dataPtr)[i] = 0xff;
		}
	}

	void Release()
	{
		if (m_dataPtr != nullptr)
		{
			delete[] m_dataPtr;
			m_dataPtr = nullptr;
		}
	}

	void SetData(GLuint offset, GLuint length, void* data)
	{
		if ((offset + length) <= m_dataSize)
		{
			GLubyte* offsetDataPtr = reinterpret_cast<GLubyte*>(m_dataPtr) + offset;
			memcpy(offsetDataPtr, data, length);
		}
	}

	inline const void* data() { return m_dataPtr; }
	inline GLsizeiptr dataSize() { return m_dataSize; }
private:
	void* m_dataPtr;

	GLsizeiptr m_dataSize;
};
