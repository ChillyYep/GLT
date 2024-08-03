#pragma once
#include <Singleton.h>
class MemoryManager :public Singleton<MemoryManager>
{
public:
	~MemoryManager();
	template<typename T, typename... TParams>
	T* New(TParams... args)
	{
		return new T(args...);
	}

	template<typename T>
	void Delete(T* t)
	{
		delete t;
	}
private:
};

