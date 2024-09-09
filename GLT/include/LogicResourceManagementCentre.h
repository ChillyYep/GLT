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

class LogicResourceManagementCentre :public Singleton<LogicResourceManagementCentre>
{
public:
	LogicResourceManagementCentre() {}
	~LogicResourceManagementCentre() {}

	Object* addResource(ResourceType resourceType, Object* resource)
	{
		switch (resourceType)
		{
		case ResourceType::Mesh:
			m_meshManagementCentre.add(static_cast<Mesh*>(resource));
			break;
		case ResourceType::Texture:
			m_textureManagementCentre.add(static_cast<Texture*>(resource));
			break;
		case ResourceType::Sampler:
			m_samplerManagementCentre.add(static_cast<Sampler*>(resource));
			break;
		case ResourceType::RenderBuffer:
			m_renderBufferManagementCentre.add(static_cast<RenderBuffer*>(resource));
			break;
		case ResourceType::RenderTarget:
			m_renderTargetManagementCentre.add(static_cast<RenderTarget*>(resource));
			break;
		default:
			break;
		}
		return resource;
	}

	RenderBuffer* addResource(RenderBufferDescriptor& renderBufferDesc)
	{
		RenderBuffer* renderBufferPtr = new RenderBuffer(renderBufferDesc);
		m_renderBufferManagementCentre.add(renderBufferPtr);
		return renderBufferPtr;
	}

	RenderTarget* addResource(RenderTargetDescriptor& renderTargetDesc)
	{
		RenderTarget* renderTargetPtr = new RenderTarget(renderTargetDesc);
		m_renderTargetManagementCentre.add(renderTargetPtr);
		return renderTargetPtr;
	}

	void destroyResource(ResourceType resouceType, Object* resource)
	{
		switch (resouceType)
		{
		case ResourceType::Mesh:
		{
			if (m_meshManagementCentre.getRefObject(resource->getInstanceId()).m_target != nullptr)
			{
				m_meshManagementCentre.remove(static_cast<Mesh*>(resource));
			}
			break;
		}
		case ResourceType::Texture:
		{
			if (m_textureManagementCentre.getRefObject(resource->getInstanceId()).m_target != nullptr)
			{
				m_textureManagementCentre.remove(static_cast<Texture*>(resource));
			}
			break;
		}
		case ResourceType::Sampler:
		{
			if (m_samplerManagementCentre.getRefObject(resource->getInstanceId()).m_target != nullptr)
			{
				m_samplerManagementCentre.remove(static_cast<Sampler*> (resource));
			}
			break;
		}
		case ResourceType::RenderBuffer:
		{
			if (m_renderBufferManagementCentre.getRefObject(resource->getInstanceId()).m_target != nullptr)
			{
				m_renderBufferManagementCentre.remove(static_cast<RenderBuffer*>(resource));
			}
			break;
		}
		case ResourceType::RenderTarget:
		{
			if (m_renderTargetManagementCentre.getRefObject(resource->getInstanceId()).m_target != nullptr)
			{
				m_renderTargetManagementCentre.remove(static_cast<RenderTarget*>(resource));
			}
			break;
		}
		default:
			break;
		}
	}

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

	void tick()
	{
		tickResources(&m_meshManagementCentre, ResourceType::Mesh);
		tickResources(&m_textureManagementCentre, ResourceType::Texture);
		tickResources(&m_samplerManagementCentre, ResourceType::Sampler);
		tickResources(&m_renderBufferManagementCentre, ResourceType::RenderBuffer);
		tickResources(&m_renderTargetManagementCentre, ResourceType::RenderTarget);
	}
	template<typename T>
	inline void tickResources(ManagementCentreBase<T*>* managementCentre, ResourceType resourceType)
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

	/// <summary>
	/// 向渲染层资源管理器推送资源
	/// </summary>
	/// <param name="resourceType"></param>
	/// <param name="resources"></param>
	void onPushResources(ResourceType resourceType, std::vector<Object*> resources)
	{
		if (resources.size() == 0)
		{
			return;
		}
		RequestResourceRef resourceRef(resources, resourceType);

		RenderEvent renderEvent;
		renderEvent.m_eventId = RenderEventId::RequestResource;
		renderEvent.m_param = &resourceRef;
		// RenderTarget属于复合资源，需要保证其依赖的资源已被申请
		if (resourceType == ResourceType::RenderTarget)
		{
			for (int i = 0; i < resources.size(); ++i)
			{
				auto rt = static_cast<RenderTarget*>(resources[i]);
				std::vector<Object*> renderBuffers;
				std::vector<Object*> textures;
				auto& attachments = rt->getAttachments();
				for (const auto& attachment : attachments)
				{
					if (attachment.getResourceType() == FBOAttachmentResourceType::Texture)
					{
						textures.push_back(attachment.getTexture());
					}
					else {
						renderBuffers.push_back(attachment.getRenderBuffer());
					}
				}
				if (renderBuffers.size() > 0)
				{
					onPushResources(ResourceType::RenderBuffer, renderBuffers);
				}
				if (textures.size() > 0)
				{
					onPushResources(ResourceType::Texture, textures);
				}
			}
		}
		RenderEventSystem::getInstance()->notify(renderEvent);
	}

	void onPopResources(ResourceType resourceType, std::vector<Object*> resources)
	{
		if (resources.size() == 0)
		{
			return;
		}
		RequestResourceRef resourceRef(resources, resourceType);

		RenderEvent renderEvent;
		renderEvent.m_eventId = RenderEventId::DestroyResource;
		renderEvent.m_param = &resourceRef;
		// RenderTarget属于复合资源，需要保证其依赖的资源已被申请
		if (resourceType == ResourceType::RenderTarget)
		{
			for (int i = 0; i < resources.size(); ++i)
			{
				auto rt = static_cast<RenderTarget*>(resources[i]);
				std::vector<Object*> renderBuffers;
				std::vector<Object*> textures;
				auto& attachments = rt->getAttachments();
				for (const auto& attachment : attachments)
				{
					if (attachment.getResourceType() == FBOAttachmentResourceType::Texture)
					{
						textures.push_back(attachment.getTexture());
					}
					else {
						renderBuffers.push_back(attachment.getRenderBuffer());
					}
				}
				if (renderBuffers.size() > 0)
				{
					onPopResources(ResourceType::RenderBuffer, renderBuffers);
				}
				if (textures.size() > 0)
				{
					onPopResources(ResourceType::Texture, textures);
				}
			}
		}
		RenderEventSystem::getInstance()->notify(renderEvent);
	}
private:
	std::vector<Object*> m_newList;
	std::vector<Object*> m_removedList;

	MeshManagementCentre m_meshManagementCentre;
	TextureManagementCentre m_textureManagementCentre;
	RenderTargetManagementCentre m_renderTargetManagementCentre;
	SamplerManagementCentre m_samplerManagementCentre;
	RenderBufferManagementCentre m_renderBufferManagementCentre;
};