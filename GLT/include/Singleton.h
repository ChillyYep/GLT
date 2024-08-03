#pragma once
template<typename T>
class Singleton
{
public:
	Singleton()
	{
		// ��֤����һ��ʵ��
		assert(m_singleton == nullptr && "There can be only one singleton");
		m_singleton = static_cast<T*>(this);
	}
	Singleton(const Singleton<T>&) = delete;
	Singleton& operator=(const Singleton<T>&) = delete;

	inline static T* getInstance() {

		static T instance;
		// ��֤����һ��ʵ��
		assert(m_singleton == &instance && "There can be only one singleton");
		return &instance;
	}
protected:
	static T* m_singleton;
private:
};


template<typename T>
T* Singleton<T>::m_singleton;
