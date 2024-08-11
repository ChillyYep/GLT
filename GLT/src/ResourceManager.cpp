#include "ResourceManager.h"

void ResourceManager::requestMeshResource(const MeshManagementCentre& meshManagementCentre, std::vector<GLuint>& meshInstanceIds)
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

void ResourceManager::destroyMeshResource(const MeshManagementCentre& meshManagementCentre, std::vector<GLuint>& meshInstanceIds)
{
	GLsizei removedCount = (GLsizei)meshInstanceIds.size();
	std::vector<MeshResourceIdentifier> identifiers = std::vector<MeshResourceIdentifier>(removedCount);

	for (size_t i = 0;i < removedCount;++i)
	{
		auto instanceId = meshInstanceIds[i];
		auto targetIter = m_meshResources.find(instanceId);
		if (targetIter == m_meshResources.end())
		{
			continue;
		}
		auto meshResourceIdentifier = (*targetIter).second;

		identifiers[i] = meshResourceIdentifier;
	}
	m_device->destroyMeshResources(identifiers);
}

void ResourceManager::requestTextureResource(const TextureManagementCentre& textureManagementCentre, std::vector<GLuint>& textureInstanceIds)
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

void ResourceManager::destroyTextureResource(const TextureManagementCentre& textureManagementCentre, std::vector<GLuint>& textureInstanceIds)
{
	GLsizei removedCount = (GLsizei)textureInstanceIds.size();
	std::vector<TextureResourceIdentifier> identifiers = std::vector<TextureResourceIdentifier>(removedCount);

	for (size_t i = 0;i < removedCount;++i)
	{
		auto instanceId = textureInstanceIds[i];
		auto targetIter = m_textureResources.find(instanceId);
		if (targetIter == m_textureResources.end())
		{
			continue;
		}
		auto textureResourceIdentifier = (*targetIter).second;

		identifiers[i] = textureResourceIdentifier;
	}
	m_device->destroyTextureResources(identifiers);
}

void ResourceManager::requestSamplerResources(const SamplerManagementCentre& samplerManagementCentre, std::vector<GLuint>& samplerInstanceIds)
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

void ResourceManager::destroySamplerResources(const SamplerManagementCentre& samplerManagementCentre, std::vector<GLuint>& samplerInstanceIds)
{
	GLsizei removedCount = (GLsizei)samplerInstanceIds.size();
	std::vector<SamplerResouceIdentifier> identifiers = std::vector<SamplerResouceIdentifier>(removedCount);

	for (size_t i = 0;i < removedCount;++i)
	{
		auto instanceId = samplerInstanceIds[i];
		auto targetIter = m_samplerResources.find(instanceId);
		if (targetIter == m_samplerResources.end())
		{
			continue;
		}
		auto samplerResourceIdentifier = (*targetIter).second;

		identifiers[i] = samplerResourceIdentifier;
	}
	m_device->destroySamplerResources(identifiers);
}

void ResourceManager::requestRenderTargetResource(const RenderTargetManagementCentre& renderTargetManagementCentre, std::vector<GLuint>& renderTargetInstanceIds)
{
	std::vector<GLuint>&& newRTInstances = filterNewIdentifier(renderTargetInstanceIds, m_renderTargetResources);
	std::vector<RenderTarget*> rtPtrs = std::vector<RenderTarget*>(newRTInstances.size());
	for (int i = 0;i < newRTInstances.size();++i)
	{
		auto rtRef = renderTargetManagementCentre.getRefObject(newRTInstances[i]).m_target;
		if (rtRef == nullptr)
		{
			continue;
		}
		rtPtrs[i] = rtRef.get();
	}

	auto rtResourceIdentifiers = m_device->requestRenderTargetResource(rtPtrs);

	for (int i = 0;i < rtResourceIdentifiers.size();++i)
	{
		m_renderTargetResources[rtPtrs[i]->getInstanceId()] = rtResourceIdentifiers[i];
	}
}

void ResourceManager::destroyRenderTargetResource(const RenderTargetManagementCentre& renderTargetManagementCentre, std::vector<GLuint>& renderTargetInstanceIds)
{
	GLsizei removedCount = (GLsizei)renderTargetInstanceIds.size();
	std::vector<RenderTargetIdentifier> identifiers = std::vector<RenderTargetIdentifier>(removedCount);

	for (size_t i = 0;i < removedCount;++i)
	{
		auto instanceId = renderTargetInstanceIds[i];
		auto targetIter = m_renderTargetResources.find(instanceId);
		if (targetIter == m_renderTargetResources.end())
		{
			continue;
		}
		auto rtResourceIdentifier = (*targetIter).second;

		identifiers[i] = rtResourceIdentifier;
	}
	m_device->destroyRenderTargetResource(identifiers);
}