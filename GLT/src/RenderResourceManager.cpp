#include "RenderResourceManager.h"

void RenderResourceManager::requestMeshResource(const MeshManagementCentre& meshManagementCentre, std::vector<GLuint>& meshInstanceIds)
{
	std::vector<GLuint>&& newMeshInstances = filterNewIdentifier(meshInstanceIds, m_meshResources);
	std::vector<Mesh*> meshPtrs = std::vector<Mesh*>(newMeshInstances.size());
	for (int i = 0;i < newMeshInstances.size();++i)
	{
		auto meshRef = meshManagementCentre.getRefObject(newMeshInstances[i]).m_target;
		if (meshRef == nullptr)
		{
			continue;
		}
		meshPtrs[i] = meshRef.get();
	}

	auto meshResourceIdentifiers = m_device->requestMeshResources(meshPtrs);

	for (int i = 0;i < meshResourceIdentifiers.size();++i)
	{
		m_meshResources[meshPtrs[i]->getInstanceId()] = meshResourceIdentifiers[i];
	}
}

void RenderResourceManager::destroyMeshResource(const MeshManagementCentre& meshManagementCentre, std::vector<GLuint>& meshInstanceIds)
{
	GLsizei removedCount = (GLsizei)meshInstanceIds.size();
	std::vector<MeshResourceIdentifier> identifiers = std::vector<MeshResourceIdentifier>(removedCount);

	for (size_t i = 0;i < removedCount;++i)
	{
		auto instanceId = meshInstanceIds[i];
		auto& meshResourceIdentifier = m_meshResources[instanceId];
		identifiers[i] = meshResourceIdentifier;
	}
	m_device->destroyMeshResources(identifiers);
}

void RenderResourceManager::requestTextureResource(const TextureManagementCentre& textureManagementCentre, std::vector<GLuint>& textureInstanceIds)
{
	std::vector<GLuint>&& newTextureInstances = filterNewIdentifier(textureInstanceIds, m_textureResources);
	std::vector<Texture*> texturePtrs = std::vector<Texture*>(newTextureInstances.size());
	for (int i = 0;i < newTextureInstances.size();++i)
	{
		auto textureRef = textureManagementCentre.getRefObject(newTextureInstances[i]).m_target;
		if (textureRef == nullptr)
		{
			continue;
		}
		texturePtrs[i] = textureRef.get();
	}

	auto textureResourceIdentifiers = m_device->requestTextureResources(texturePtrs);

	for (int i = 0;i < textureResourceIdentifiers.size();++i)
	{
		m_textureResources[texturePtrs[i]->getInstanceId()] = textureResourceIdentifiers[i];
	}
}

void RenderResourceManager::destroyTextureResource(const TextureManagementCentre& textureManagementCentre, std::vector<GLuint>& textureInstanceIds)
{
	GLsizei removedCount = (GLsizei)textureInstanceIds.size();
	std::vector<TextureResourceIdentifier> identifiers = std::vector<TextureResourceIdentifier>(removedCount);

	for (size_t i = 0;i < removedCount;++i)
	{
		auto instanceId = textureInstanceIds[i];
		auto& textureResourceIdentifier = m_textureResources[instanceId];
		identifiers[i] = textureResourceIdentifier;
	}
	m_device->destroyTextureResources(identifiers);
}

void RenderResourceManager::requestSamplerResources(const SamplerManagementCentre& samplerManagementCentre, std::vector<GLuint>& samplerInstanceIds)
{
	std::vector<GLuint>&& newSamplerInstances = filterNewIdentifier(samplerInstanceIds, m_samplerResources);
	std::vector<Sampler*> samplerPtrs = std::vector<Sampler*>(newSamplerInstances.size());
	for (int i = 0;i < newSamplerInstances.size();++i)
	{
		auto samplerRef = samplerManagementCentre.getRefObject(newSamplerInstances[i]).m_target;
		if (samplerRef == nullptr)
		{
			continue;
		}
		samplerPtrs[i] = samplerRef.get();
	}

	auto samplerResourceIdentifiers = m_device->requestSamplerResources(samplerPtrs);

	for (int i = 0;i < samplerResourceIdentifiers.size();++i)
	{
		m_samplerResources[samplerPtrs[i]->getInstanceId()] = samplerResourceIdentifiers[i];
	}
}

void RenderResourceManager::destroySamplerResources(const SamplerManagementCentre& samplerManagementCentre, std::vector<GLuint>& samplerInstanceIds)
{
	GLsizei removedCount = (GLsizei)samplerInstanceIds.size();
	std::vector<SamplerResouceIdentifier> identifiers = std::vector<SamplerResouceIdentifier>(removedCount);

	for (size_t i = 0;i < removedCount;++i)
	{
		auto instanceId = samplerInstanceIds[i];
		auto samplerResourceIdentifier = m_samplerResources[instanceId];
		identifiers[i] = samplerResourceIdentifier;
	}
	m_device->destroySamplerResources(identifiers);
}

void RenderResourceManager::requestRenderBufferResources(std::vector<RenderBuffer*>& renderBuffers)
{
	std::vector<GLuint> instanceIds = std::vector<GLuint>(renderBuffers.size());
	instanceIds = filterNewIdentifier(instanceIds, m_renderBufferResources);

	auto rbIdentifiers = m_device->requestRenderBufferResources(renderBuffers);

	for (int i = 0;i < rbIdentifiers.size();++i)
	{
		m_renderBufferResources[rbIdentifiers[i].getInstanceId()] = rbIdentifiers[i];
	}
}

void RenderResourceManager::destroyRenderBufferResources(std::vector<RenderBuffer*>& renderBuffers)
{
	GLsizei removedCount = (GLsizei)renderBuffers.size();
	std::vector<RenderBufferIdentifier> identifiers = std::vector<RenderBufferIdentifier>(removedCount);

	for (size_t i = 0;i < removedCount;++i)
	{
		auto instanceId = renderBuffers[i]->getInstanceId();
		auto& renderBufferResourceIdentifier = m_renderBufferResources[instanceId];
		identifiers[i] = renderBufferResourceIdentifier;
	}
	m_device->destroyRenderBufferResources(identifiers);
}

void RenderResourceManager::requestRenderTargetResource(const RenderTargetManagementCentre& renderTargetManagementCentre, std::vector<GLuint>& renderTargetInstanceIds)
{
	std::vector<GLuint>&& newRTInstances = filterNewIdentifier(renderTargetInstanceIds, m_renderTargetResources);
	std::vector<RenderTarget*> rtPtrs(newRTInstances.size());
	std::vector<std::vector<AttachmentEntityIdentifierWrapper>> rtIdentifierAttachments(newRTInstances.size());
	for (int i = 0;i < newRTInstances.size();++i)
	{
		auto rtRef = renderTargetManagementCentre.getRefObject(newRTInstances[i]).m_target;
		if (rtRef == nullptr)
		{
			continue;
		}
		rtPtrs[i] = rtRef.get();
	}
	// 收集需要申请的附件
	std::vector<RenderBuffer*> renderBuffers;
	std::vector<GLuint> textureInstances;
	for (size_t i = 0; i < rtPtrs.size(); ++i)
	{
		auto rtPtr = rtPtrs[i];
		auto attachments = rtPtr->getAttachments();
		for (const auto& attachment : attachments)
		{
			auto resourceType = attachment.getResourceType();
			if (resourceType == FBOAttachmentResourceType::RenderBuffer)
			{
				RenderBuffer* rb = attachment.getRenderBuffer();
				if (m_renderBuffers.find(rb->getInstanceId()) == m_renderBuffers.end())
				{
					m_renderBuffers[rb->getInstanceId()] = *rb;
					renderBuffers.push_back(&m_renderBuffers[rb->getInstanceId()]);
				}
			}
			else if (resourceType == FBOAttachmentResourceType::Texture)
			{
				Texture* texPtr = attachment.getTexture();
				textureInstances.push_back(texPtr->getInstanceId());
			}
		}
	}
	// 申请必要的资源
	requestRenderBufferResources(renderBuffers);
	requestTextureResource(m_textureManagementCentre, textureInstances);

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

	for (int i = 0;i < rtResourceIdentifiers.size();++i)
	{
		m_renderTargetResources[rtPtrs[i]->getInstanceId()] = rtResourceIdentifiers[i];
	}
}

void RenderResourceManager::destroyRenderTargetResource(const RenderTargetManagementCentre& renderTargetManagementCentre, std::vector<GLuint>& renderTargetInstanceIds)
{
	GLsizei removedCount = (GLsizei)renderTargetInstanceIds.size();
	std::vector<RenderTargetIdentifier> identifiers = std::vector<RenderTargetIdentifier>(removedCount);
	std::vector<RenderBuffer*> removedRenderBuffers;
	std::vector<GLuint> removedTexs;
	for (size_t i = 0;i < removedCount;++i)
	{
		auto instanceId = renderTargetInstanceIds[i];
		auto& rtResourceIdentifier = m_renderTargetResources[instanceId];

		identifiers[i] = rtResourceIdentifier;

		for (const auto& attachment : rtResourceIdentifier.m_attachmentIdentifiers)
		{
			if (attachment.getResourceType() == FBOAttachmentResourceType::RenderBuffer)
			{
				removedRenderBuffers.push_back(&m_renderBuffers[attachment.getRenderBufferIdentifier()->getInstanceId()]);
			}
			else {
				removedTexs.push_back(attachment.getTextureIdentifier()->getInstanceId());
			}
		}
	}
	// 销毁附件资源
	destroyTextureResource(m_textureManagementCentre, removedTexs);
	destroyRenderBufferResources(removedRenderBuffers);
	// 销毁FBO本体
	m_device->destroyRenderTargetResource(identifiers);
}

void RenderResourceManager::requestConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers)
{
	m_device->requestConstantBufferResources(constantBufferIdentifiers);
}

void RenderResourceManager::destroyConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers)
{
	m_device->destroyConstantBufferResources(constantBufferIdentifiers);
}