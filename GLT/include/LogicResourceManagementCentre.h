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
struct ResourceRef
{
	std::vector<Object*> m_targets;
	ResourceType m_resourceType;
};

class LogicResourceManagementCentre :public Singleton<LogicResourceManagementCentre>
{
public:
	LogicResourceManagementCentre() {}
	~LogicResourceManagementCentre() {}

	Object* addResource(ResourceType resouceType, Object* resource)
	{
		switch (resouceType)
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

	RenderBuffer* addResource(RenderBufferDesc& renderBufferDesc)
	{
		RenderBuffer* renderBufferPtr = new RenderBuffer();
		m_renderBufferManagementCentre.add(renderBufferPtr);
		return renderBufferPtr;
	}

	void destroyResource(RenderBuffer* renderBufferPtr)
	{
		m_renderBufferManagementCentre.remove(renderBufferPtr);
	}

	void destroyResource(ResourceType resouceType, Object* resource)
	{
		switch (resouceType)
		{
		case ResourceType::Mesh:
			m_meshManagementCentre.remove(static_cast<Mesh*>(resource));
			break;
		case ResourceType::Texture:
			m_textureManagementCentre.remove(static_cast<Texture*>(resource));
			break;
		case ResourceType::Sampler:
			m_samplerManagementCentre.remove(static_cast<Sampler*>(resource));
			break;
		case ResourceType::RenderBuffer:
			m_renderBufferManagementCentre.remove(static_cast<RenderBuffer*>(resource));
			break;
		case ResourceType::RenderTarget:
			m_renderTargetManagementCentre.remove(static_cast<RenderTarget*>(resource));
			break;
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

	/// <summary>
	/// 向渲染层资源管理器推送资源
	/// </summary>
	/// <param name="resourceType"></param>
	/// <param name="resources"></param>
	void onPushResources(ResourceType resourceType, std::vector<Object*> resources)
	{
		ResourceRef resourceRef;
		resourceRef.m_resourceType = resourceType;
		resourceRef.m_targets = resources;

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
private:
	MeshManagementCentre m_meshManagementCentre;
	TextureManagementCentre m_textureManagementCentre;
	RenderTargetManagementCentre m_renderTargetManagementCentre;
	SamplerManagementCentre m_samplerManagementCentre;
	RenderBufferManagementCentre m_renderBufferManagementCentre;
};