#pragma once
#include <Singleton.h>
#include <EventObserver.h>
#include <MeshManagementCentre.h>
#include <TextureManagementCentre.h>
#include <SamplerManagementCentre.h>
#include <RenderTargetManagementCentre.h>
#include <RenderBufferManagementCentre.h>
#include <RenderEventSystem.h>

enum class ResourceType {
	Mesh,
	Texture,
	Sampler,
	RenderBuffer,
	RenderTarget,
};
struct RequestResourceRef
{
	RequestResourceRef(std::vector<Object*> targets, ResourceType resourceType)
		:m_targets(targets), m_resourceType(resourceType) {}
	std::vector<Object*> m_targets;
	ResourceType m_resourceType;
};

class LogicResourceManager :public Singleton<LogicResourceManager>
{
public:
	LogicResourceManager() {}
	~LogicResourceManager() {}

	Object* addResource(ResourceType resourceType, Object* resource);

	RenderBuffer* addResource(RenderBufferDescriptor& renderBufferDesc);

	RenderTarget* addResource(RenderTargetDescriptor& renderTargetDesc);

	void destroyResource(ResourceType resouceType, Object* resource);

	template<typename T>
	std::shared_ptr<T> getResource(ResourceType resourceType, unsigned int instanceId)
	{
		switch (resourceType)
		{
		case ResourceType::Mesh:
			return m_meshManagementCentre.getRefObject(instanceId).m_target;
		case ResourceType::Texture:
			return m_textureManagementCentre.getRefObject(instanceId).m_target;
		case ResourceType::Sampler:
			return m_samplerManagementCentre.getRefObject(instanceId).m_target;
		case ResourceType::RenderBuffer:
			return m_renderBufferManagementCentre.getRefObject(instanceId).m_target;
		case ResourceType::RenderTarget:
			return m_renderTargetManagementCentre.getRefObject(instanceId).m_target;
		default:
			break;
		}
	}
	
	template<typename T>
	inline void tickResourcesQueue(ManagementCentreBase<T*>* managementCentre, ResourceType resourceType)
	{
		managementCentre->getChangedObjects(m_newList, m_removedList);
		onPushResources(resourceType, m_newList);
		onPopResources(resourceType, m_removedList);

		for (int i = 0; i < m_removedList.size(); ++i)
		{
			delete m_removedList[i];
		}

		m_newList.clear();
		m_removedList.clear();

		managementCentre->clearChangedObjectfs();
	}

	void tick();

	/// <summary>
	/// 向渲染层资源管理器推送资源
	/// </summary>
	/// <param name="resourceType"></param>
	/// <param name="resources"></param>
	void onPushResources(ResourceType resourceType, std::vector<Object*> resources);

	/// <summary>
	/// 从渲染层资源管理器销毁资源
	/// </summary>
	/// <param name="resourceType"></param>
	/// <param name="resources"></param>
	void onPopResources(ResourceType resourceType, std::vector<Object*> resources);
private:
	std::vector<Object*> m_newList;
	std::vector<Object*> m_removedList;

	MeshManagementCentre m_meshManagementCentre;
	TextureManagementCentre m_textureManagementCentre;
	RenderTargetManagementCentre m_renderTargetManagementCentre;
	SamplerManagementCentre m_samplerManagementCentre;
	RenderBufferManagementCentre m_renderBufferManagementCentre;
};