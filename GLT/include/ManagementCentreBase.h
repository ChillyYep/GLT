#pragma once
#include <CommonDefine.h>
#include <Object.h>
#include <map>
#include <vector>
#include <set>
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
	virtual ~ManagementCentreBase() {}

	virtual void add(T target)
	{
		auto instanceId = target->getInstanceId();
		assert(instanceId > 0);
		if (m_allObjects.find(instanceId) == m_allObjects.end())
		{
			m_allObjects[instanceId] = ReferencedObject(target);
			m_sortedObjects.push_back(instanceId);
			m_addSet.insert(instanceId);
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
			m_removeDict[instanceId] = m_allObjects[instanceId].m_target;
			m_allObjects.erase(instanceId);
		}
	}

	void getChangedObjects(std::vector<Object*>& addedList, std::vector<Object*>& removedList)
	{
		// 同一帧添加和删除视为抵消操作
		for (const auto& addItem : m_addSet)
		{
			// 保证不在删除集合中
			if (m_removeDict.find(addItem) == m_removeDict.end())
			{
				addedList.push_back(m_allObjects[addItem].m_target);
			}
		}
		for (const auto& removedtem : m_removeDict)
		{
			// 保证不在添加集合中
			if (m_addSet.find(removedtem.first) == m_addSet.end())
			{
				removedList.push_back(removedtem.second);
			}
		}
	}

	void clearChangedObjectfs()
	{
		m_removeDict.clear();
		m_addSet.clear();
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
protected:
	std::map<GLTUInt32, ReferencedObject> m_allObjects;

	std::map<GLTUInt32, Object*> m_removeDict;

	std::set<GLTUInt32> m_addSet;

	std::vector<GLTUInt32> m_sortedObjects;
};
