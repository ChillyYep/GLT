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

	virtual void Add(T target)
	{
		auto instanceId = target->GetInstanceId();
		assert(instanceId > 0);
		if (m_allObjects.find(instanceId) == m_allObjects.end())
		{
			m_allObjects[instanceId] = ReferencedObject(target);
			m_sortedObjects.push_back(instanceId);
			m_addList.push_back(instanceId);
		}
		m_allObjects[instanceId].m_refCount++;
	}

	virtual void Remove(T target)
	{
		auto instanceId = target->GetInstanceId();
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

	virtual void OnSubmit() {}

	inline std::vector<GLTUInt32>& GetAllObjectInstanceIds() const { return m_sortedObjects; }

	inline ReferencedObject GetRefObject(GLTUInt32 instanceId) const
	{
		auto iter = m_allObjects.find(instanceId);
		if (m_allObjects.end() != iter)
		{
			return (*iter).second;
		}
		return ReferencedObject();
	}

	inline std::vector<ReferencedObject> GetRemovedObjects() const { return m_removeList; }

	inline std::vector<GLTUInt32> GetExpiredObjectInstanceIds() const
	{
		std::vector<GLTUInt32> removedInstanceIds;
		for (int i = 0;i < m_removeList.size();++i)
		{
			removedInstanceIds.push_back(m_removeList[i].m_target->GetInstanceId());
		}
		return removedInstanceIds;
	}

	inline void ClearExpiredList() { m_removeList.clear(); }

	inline std::vector<GLTUInt32> GetNewObjects() const { return m_addList; }

	inline void ClearNewList() { m_addList.clear(); }
protected:
	std::map<GLTUInt32, ReferencedObject> m_allObjects;

	std::vector<ReferencedObject> m_removeList;

	std::vector<GLTUInt32> m_addList;

	std::vector<GLTUInt32> m_sortedObjects;
};
