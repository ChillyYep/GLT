#include "LogicResourceManager.h"

Object* LogicResourceManager::addResource(ResourceType resourceType, Object* resource)
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

RenderBuffer* LogicResourceManager::addResource(RenderBufferDescriptor& renderBufferDesc)
{
	RenderBuffer* renderBufferPtr = new RenderBuffer(renderBufferDesc);
	m_renderBufferManagementCentre.add(renderBufferPtr);
	return renderBufferPtr;
}

RenderTarget* LogicResourceManager::addResource(RenderTargetDescriptor& renderTargetDesc)
{
	RenderTarget* renderTargetPtr = new RenderTarget(renderTargetDesc);
	m_renderTargetManagementCentre.add(renderTargetPtr);
	return renderTargetPtr;
}

void LogicResourceManager::destroyResource(ResourceType resouceType, Object* resource)
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

void LogicResourceManager::tick()
{
	tickResourcesQueue(&m_meshManagementCentre, ResourceType::Mesh);
	tickResourcesQueue(&m_textureManagementCentre, ResourceType::Texture);
	tickResourcesQueue(&m_samplerManagementCentre, ResourceType::Sampler);
	tickResourcesQueue(&m_renderBufferManagementCentre, ResourceType::RenderBuffer);
	tickResourcesQueue(&m_renderTargetManagementCentre, ResourceType::RenderTarget);
}

void LogicResourceManager::onPushResources(ResourceType resourceType, std::vector<Object*> resources)
{
	if (resources.size() == 0)
	{
		return;
	}
	RequestResourceRef resourceRef(resources, resourceType);

	RenderEvent renderEvent;
	renderEvent.m_eventId = RenderEventId::RequestResources;
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

void LogicResourceManager::onUpdateResources(ResourceType resourceType, std::vector<Object*> resources)
{
	if (resources.size() == 0)
	{
		return;
	}
	RequestResourceRef resourceRef(resources, resourceType);

	RenderEvent renderEvent;
	renderEvent.m_eventId = RenderEventId::UpdateResources;
	renderEvent.m_param = &resourceRef;
	// RenderTarget属于复合资源，需要保证其依赖的资源已被申请
	//if (resourceType == ResourceType::RenderTarget)
	//{
	//	for (int i = 0; i < resources.size(); ++i)
	//	{
	//		auto rt = static_cast<RenderTarget*>(resources[i]);
	//		std::vector<Object*> renderBuffers;
	//		std::vector<Object*> textures;
	//		auto& attachments = rt->getAttachments();
	//		for (const auto& attachment : attachments)
	//		{
	//			if (attachment.getResourceType() == FBOAttachmentResourceType::Texture)
	//			{
	//				textures.push_back(attachment.getTexture());
	//			}
	//			else {
	//				renderBuffers.push_back(attachment.getRenderBuffer());
	//			}
	//		}
	//		if (renderBuffers.size() > 0)
	//		{
	//			onUpdateResources(ResourceType::RenderBuffer, renderBuffers);
	//		}
	//		if (textures.size() > 0)
	//		{
	//			onUpdateResources(ResourceType::Texture, textures);
	//		}
	//	}
	//}
	RenderEventSystem::getInstance()->notify(renderEvent);
}

void LogicResourceManager::onPopResources(ResourceType resourceType, std::vector<Object*> resources)
{
	if (resources.size() == 0)
	{
		return;
	}
	RequestResourceRef resourceRef(resources, resourceType);

	RenderEvent renderEvent;
	renderEvent.m_eventId = RenderEventId::DestroyResources;
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