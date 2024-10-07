#include "RenderResourceManagment.h"

void RenderResourceManagement::onNotify(RenderEvent& renderEvent)
{
	if (renderEvent.m_eventId == RenderEventId::RequestResources)
	{
		auto resourceRef = static_cast<RequestResourceRef*>(renderEvent.m_param);
		generateResourceIdentifier(resourceRef->m_resourceType, resourceRef->m_targets);
	}
	else if (renderEvent.m_eventId == RenderEventId::DestroyResources)
	{
		auto resourceRef = static_cast<RequestResourceRef*>(renderEvent.m_param);
		destroyResourceIdentifier(resourceRef->m_resourceType, resourceRef->m_targets);
	}
	else if (renderEvent.m_eventId == RenderEventId::UpdateResources)
	{
		auto resourceRef = static_cast<RequestResourceRef*>(renderEvent.m_param);
		updateResouceIdentifier(resourceRef->m_resourceType, resourceRef->m_targets);
	}
}

ResourceIdentifier* RenderResourceManagement::getResourceIdentifier(ResourceType resourceType, unsigned int instanceId)
{
	switch (resourceType)
	{
	case ResourceType::Mesh:
	{
		if (m_meshResources.find(instanceId) != m_meshResources.end())
		{
			return &m_meshResources[instanceId];
		}
		break;
	}
	case ResourceType::Texture:
	{
		if (m_textureResources.find(instanceId) != m_textureResources.end())
		{
			return &m_textureResources[instanceId];
		}
		break;
	}
	case ResourceType::Sampler:
	{
		if (m_samplerResources.find(instanceId) != m_samplerResources.end())
		{
			return &m_samplerResources[instanceId];
		}
		break;
	}
	case ResourceType::RenderBuffer:
	{
		if (m_renderBufferResources.find(instanceId) != m_renderBufferResources.end())
		{
			return &m_renderBufferResources[instanceId];
		}
		break;
	}
	case ResourceType::RenderTarget:
	{
		if (m_renderTargetResources.find(instanceId) != m_renderTargetResources.end())
		{
			return &m_renderTargetResources[instanceId];
		}
		break;
	}
	default:
		break;
	}
	return nullptr;
}

void RenderResourceManagement::generateResourceIdentifier(ResourceType resourceType, std::vector<Object*>& objects)
{
	switch (resourceType)
	{
	case ResourceType::Mesh:
	{
		std::vector<Mesh*>&& meshPtrs = filterNewResources<Mesh>(objects, m_meshResources);

		if (meshPtrs.size() > 0)
		{
			auto meshResourceIdentifiers = m_device->requestMeshResources(meshPtrs);

			for (int i = 0; i < meshResourceIdentifiers.size(); ++i)
			{
				m_meshResources[meshPtrs[i]->getInstanceId()] = meshResourceIdentifiers[i];
			}
		}
		break;
	}
	case ResourceType::Texture:
	{
		std::vector<Texture*>&& texturePtrs = filterNewResources<Texture>(objects, m_textureResources);

		if (texturePtrs.size() > 0)
		{
			auto textureResourceIdentifiers = m_device->requestTextureResources(texturePtrs);

			for (int i = 0; i < textureResourceIdentifiers.size(); ++i)
			{
				m_textureResources[texturePtrs[i]->getInstanceId()] = textureResourceIdentifiers[i];
			}
		}

		break;
	}
	case ResourceType::Sampler:
	{
		std::vector<Sampler*>&& samplerPtrs = filterNewResources<Sampler>(objects, m_samplerResources);

		if (samplerPtrs.size() > 0)
		{
			auto samplerResourceIdentifiers = m_device->requestSamplerResources(samplerPtrs);

			for (int i = 0; i < samplerResourceIdentifiers.size(); ++i)
			{
				m_samplerResources[samplerPtrs[i]->getInstanceId()] = samplerResourceIdentifiers[i];
			}
		}
		break;
	}
	case ResourceType::RenderBuffer:
	{
		std::vector<RenderBuffer*>&& renderBufferPtrs = filterNewResources<RenderBuffer>(objects, m_renderBufferResources);

		if (renderBufferPtrs.size() > 0)
		{
			auto renderBufferResourceIdentifiers = m_device->requestRenderBufferResources(renderBufferPtrs);

			for (int i = 0; i < renderBufferResourceIdentifiers.size(); ++i)
			{
				m_renderBufferResources[renderBufferPtrs[i]->getInstanceId()] = renderBufferResourceIdentifiers[i];
			}
		}
		break;
	}
	case ResourceType::RenderTarget:
	{
		std::vector<RenderTarget*>&& rtPtrs = filterNewResources<RenderTarget>(objects, m_renderTargetResources);

		if (rtPtrs.size() > 0)
		{
			std::vector<std::vector<AttachmentEntityIdentifierWrapper>> rtIdentifierAttachments(rtPtrs.size());

			// 生成附件标识符
			for (size_t i = 0; i < rtPtrs.size(); ++i)
			{
				auto rtPtr = rtPtrs[i];
				auto attachments = rtPtr->getAttachments();
				std::vector<AttachmentEntityIdentifierWrapper> identifierWrappers;
				for (const auto& attachment : attachments)
				{
					auto resourceType = attachment.getResourceType();
					if (resourceType == FBOAttachmentResourceType::RenderBuffer)
					{
						RenderBuffer* rb = attachment.getRenderBuffer();
						if (m_renderBufferResources.find(rb->getInstanceId()) != m_renderBufferResources.end())
						{
							identifierWrappers.push_back(AttachmentEntityIdentifierWrapper(&m_renderBufferResources[rb->getInstanceId()], attachment.getAttachmentType()));
						}
					}
					else if (resourceType == FBOAttachmentResourceType::Texture)
					{
						Texture* texPtr = attachment.getTexture();
						if (m_textureResources.find(texPtr->getInstanceId()) != m_textureResources.end())
						{
							identifierWrappers.push_back(AttachmentEntityIdentifierWrapper(&m_textureResources[texPtr->getInstanceId()], attachment.getAttachmentType()));
						}
					}
				}
				rtIdentifierAttachments[i] = identifierWrappers;
			}

			auto rtResourceIdentifiers = m_device->requestRenderTargetResource(rtPtrs, rtIdentifierAttachments);

			for (int i = 0; i < rtResourceIdentifiers.size(); ++i)
			{
				m_renderTargetResources[rtPtrs[i]->getInstanceId()] = rtResourceIdentifiers[i];
			}
		}
		break;
	}
	default:
		break;
	}
}

void RenderResourceManagement::destroyResourceIdentifier(ResourceType resourceType, std::vector<Object*>& objects)
{
	auto&& instanceIds = getInstanceIds(objects);
	auto removedCount = instanceIds.size();
	switch (resourceType)
	{
	case ResourceType::Mesh:
	{
		std::vector<MeshResourceIdentifier>&& identifiers = getResourceIdentifier(instanceIds, m_meshResources);
		if (identifiers.size() > 0)
		{
			m_device->destroyMeshResources(identifiers);
			for (int i = 0; i < instanceIds.size(); ++i)
			{
				m_meshResources.erase(instanceIds[i]);
			}
		}
		break;
	}
	case ResourceType::Texture:
	{
		std::vector<TextureResourceIdentifier>&& identifiers = getResourceIdentifier(instanceIds, m_textureResources);
		if (identifiers.size() > 0)
		{
			m_device->destroyTextureResources(identifiers);
			for (int i = 0; i < instanceIds.size(); ++i)
			{
				m_textureResources.erase(instanceIds[i]);
			}
			break;
		}
	}
	case ResourceType::Sampler:
	{
		std::vector<SamplerResouceIdentifier>&& identifiers = getResourceIdentifier(instanceIds, m_samplerResources);
		if (identifiers.size() > 0)
		{
			m_device->destroySamplerResources(identifiers);
			for (int i = 0; i < instanceIds.size(); ++i)
			{
				m_samplerResources.erase(instanceIds[i]);
			}
		}
		break;
	}
	case ResourceType::RenderBuffer:
	{
		std::vector<RenderBufferIdentifier>&& identifiers = getResourceIdentifier(instanceIds, m_renderBufferResources);
		if (identifiers.size() > 0)
		{
			m_device->destroyRenderBufferResources(identifiers);
			for (int i = 0; i < instanceIds.size(); ++i)
			{
				m_renderBufferResources.erase(instanceIds[i]);
			}
		}
		break;
	}
	case ResourceType::RenderTarget:
	{
		std::vector<RenderTargetIdentifier>&& identifiers = getResourceIdentifier(instanceIds, m_renderTargetResources);
		if (identifiers.size() > 0)
		{
			m_device->destroyRenderTargetResource(identifiers);
			for (int i = 0; i < instanceIds.size(); ++i)
			{
				m_renderTargetResources.erase(instanceIds[i]);
			}
		}
		break;
	}
	default:
		break;
	}
}

void RenderResourceManagement::updateResouceIdentifier(ResourceType resourceType, std::vector<Object*>& objects)
{
	auto&& instanceIds = getInstanceIds(objects);
	auto removedCount = instanceIds.size();
	switch (resourceType)
	{
	case ResourceType::Texture:
	{
		std::vector<TextureResourceIdentifier>&& identifiers = getResourceIdentifier(instanceIds, m_textureResources);
		std::vector<Texture*> textures(objects.size());
		for (int i = 0;i < objects.size();++i)
		{
			textures[i] = static_cast<Texture*>(objects[i]);
		}
		if (identifiers.size() > 0 && textures.size() > 0)
		{
			m_device->updateTextureResources(textures, identifiers);
			break;
		}
	}
	default:
		assert(false && "updateResouceIdentifier not support!");
		break;
	}
}