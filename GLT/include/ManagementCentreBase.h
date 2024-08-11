#pragma once
#include <CommonDefine.h>
#include <map>
#include <vector>
template<typename T>
class ReferenceItem {
public:
	ReferenceItem() :m_refCount(0) {}
	ReferenceItem(T target) :m_target(target), m_refCount(0) {}
	T m_target;
	GLTUInt32 m_refCount;
};

/// <summary>
/// 用于CPU和GPU需要同步的情况
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class ManagementCentreBase
{
public:
	typedef ReferenceItem<T> ReferencedObject;
	ManagementCentreBase() {}
	~ManagementCentreBase() {}

	virtual void add(T target)
	{
		auto instanceId = target->getInstanceId();
		assert(instanceId > 0);
		if (m_allObjects.find(instanceId) == m_allObjects.end())
		{
			m_allObjects[instanceId] = ReferencedObject(target);
			m_sortedObjects.push_back(instanceId);
			m_addList.push_back(instanceId);
		}
		m_allObjects[instanceId].m_refCount++;
	}

	virtual void remove(T target)
	{
		auto instanceId = target->getInstanceId();
		assert(instanceId > 0);
		if (m_allObjects.find(instanceId) != m_allObjects.end())
		{
			m_allObjects[instanceId].m_refCount--;
		}
		if (m_allObjects[instanceId].m_refCount == 0)
		{
			m_removeList.push_back(ReferencedObject(target));
			m_allObjects.erase(instanceId);
		}
	}

	virtual void onSubmit() {}

	inline std::vector<GLTUInt32> getAllObjectInstanceIds() const { return m_sortedObjects; }

	inline ReferencedObject getRefObject(GLTUInt32 instanceId) const
	{
		auto iter = m_allObjects.find(instanceId);
		if (m_allObjects.end() != iter)
		{
			return (*iter).second;
		}
		return ReferencedObject();
	}

	inline std::vector<ReferencedObject> getRemovedObjects() const { return m_removeList; }

	inline std::vector<GLTUInt32> getExpiredObjectInstanceIds() const
	{
		std::vector<GLTUInt32> removedInstanceIds;
		for (int i = 0;i < m_removeList.size();++i)
		{
			removedInstanceIds.push_back(m_removeList[i].m_target->getInstanceId());
		}
		return removedInstanceIds;
	}

	inline void clearExpiredList() { m_removeList.clear(); }

	inline std::vector<GLTUInt32> getNewObjects() const { return m_addList; }

	inline void clearNewList() { m_addList.clear(); }
protected:
	std::map<GLTUInt32, ReferencedObject> m_allObjects;

	std::vector<ReferencedObject> m_removeList;

	std::vector<GLTUInt32> m_addList;

	std::vector<GLTUInt32> m_sortedObjects;
};
