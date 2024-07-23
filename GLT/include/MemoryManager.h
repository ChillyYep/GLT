#pragma once

class MemoryManager
{
public:
	MemoryManager(const MemoryManager&) = delete;
	MemoryManager& operator=(const MemoryManager&) = delete;
	~MemoryManager();
	static MemoryManager& GetInstance()
	{
		static MemoryManager memoryManager;
		return memoryManager;
	}

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
	MemoryManager();
};

