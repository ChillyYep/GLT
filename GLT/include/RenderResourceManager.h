#pragma once
#include <CommonDefine.h>
#include <Texture.h>
#include <ResourceIdentifier.h>
#include <ConstantBufferIdentifier.h>
#include <MeshManagementCentre.h>
#include <TextureManagementCentre.h>
#include <SamplerManagementCentre.h>
#include <RenderTargetManagementCentre.h>
#include <Singleton.h>
#include <iostream>
#include <GLCommon.h>
#include <DeviceBase.h>

class RenderResourceManager :public Singleton<RenderResourceManager>
{
public:
	~RenderResourceManager() {}

	template<typename T>
	inline void requestResource(ManagementCentreBase<T>* managementCentre, std::vector<GLuint> instanceIds, ResourceType resourceType)
	{
		switch (resourceType)
		{
		case ResourceType::Mesh:
			requestMeshResource(*dynamic_cast<MeshManagementCentre*>(managementCentre), instanceIds);
			break;
		case ResourceType::Texture:
			requestTextureResource(*dynamic_cast<TextureManagementCentre*>(managementCentre), instanceIds);
			break;
		case ResourceType::Sampler:
			requestSamplerResources(*dynamic_cast<SamplerManagementCentre*>(managementCentre), instanceIds);
			break;
		case ResourceType::RenderTarget:
			requestRenderTargetResource(*dynamic_cast<RenderTargetManagementCentre*>(managementCentre), instanceIds);
			break;
		default:
			break;
		}
	}

	template<typename T>
	inline void destroyResource(ManagementCentreBase<T>* managementCentre, std::vector<GLuint> instanceIds, ResourceType resourceType)
	{
		switch (resourceType)
		{
		case ResourceType::Mesh:
			destroyMeshResource(*dynamic_cast<MeshManagementCentre*>(managementCentre), instanceIds);
			break;
		case ResourceType::Texture:
			destroyTextureResource(*dynamic_cast<TextureManagementCentre*>(managementCentre), instanceIds);
			break;
		case ResourceType::Sampler:
			destroySamplerResources(*dynamic_cast<SamplerManagementCentre*>(managementCentre), instanceIds);
			break;
		case ResourceType::RenderTarget:
			destroyRenderTargetResource(*dynamic_cast<RenderTargetManagementCentre*>(managementCentre), instanceIds);
			break;
		default:
			break;
		}
	}

	void requestMeshResource(const MeshManagementCentre& meshManagementCentre, std::vector<GLuint>& meshInstanceIds);

	void destroyMeshResource(const MeshManagementCentre& meshManagementCentre, std::vector<GLuint>& meshInstanceIds);

	void requestTextureResource(const TextureManagementCentre& textureManagementCentre, std::vector<GLuint>& textureInstanceIds);

	void destroyTextureResource(const TextureManagementCentre& textureManagementCentre, std::vector<GLuint>& textureInstanceIds);

	void requestSamplerResources(const SamplerManagementCentre& samplerManagementCentre, std::vector<GLuint>& samplerInstanceIds);

	void destroySamplerResources(const SamplerManagementCentre& samplerManagementCentre, std::vector<GLuint>& samplerInstanceIds);

	void requestConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers);

	void destroyConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers);

	void uploadConstantBufferResource(ConstantBufferType constantBufferType)
	{
		m_device->uploadConstantBufferResource(constantBufferType);
	}

	void requestRenderTargetResource(const RenderTargetManagementCentre& renderTargetManagementCentre, std::vector<GLuint>& renderTargetInstanceIds);

	void destroyRenderTargetResource(const RenderTargetManagementCentre& renderTargetManagementCentre, std::vector<GLuint>& textureInstanceIds);

	void requestRenderBufferResources(std::vector<RenderBuffer*>& renderBufferIdentifiers);

	void destroyRenderBufferResources(std::vector<RenderBuffer*>& renderBufferIdentifiers);

	inline MeshResourceIdentifier* getMeshResource(GLuint meshInstanceId)
	{
		return getResource(meshInstanceId, m_meshResources);
	}

	inline TextureResourceIdentifier* getTextureResource(GLuint textureInstanceId)
	{
		return getResource(textureInstanceId, m_textureResources);
	}

	inline RenderTargetIdentifier* getRenderTargetResource(GLuint renderTextureInstanceId)
	{
		return getResource(renderTextureInstanceId, m_renderTargetResources);
	}

	inline MeshManagementCentre& getMeshManagementCentre() { return m_meshManagementCentre; }

	inline TextureManagementCentre& getTextureManagementCentre() { return m_textureManagementCentre; }

	inline RenderTargetManagementCentre& getRenderTargetManagementCentre() { return m_renderTargetManagementCentre; }

	inline std::unordered_map<GLuint, MeshResourceIdentifier>& getMeshResources() { return m_meshResources; }

	inline std::unordered_map<GLuint, TextureResourceIdentifier>& getTextureResources() { return m_textureResources; }

	inline std::unordered_map<GLuint, RenderTargetIdentifier>& getRenderTargetResources() { return m_renderTargetResources; }

	inline void SetDevice(DeviceBase* device)
	{
		m_device = device;
	}
private:
	template<typename Identifier>
	std::vector<GLuint> filterNewIdentifier(std::vector<GLuint>& instanceIds, std::unordered_map<GLuint, Identifier>& identifiers)
	{
		size_t length = instanceIds.size();
		std::vector<GLuint> newInstanceIds;
		GLuint newInstanceCount = 0;
		for (size_t i = 0;i < length;++i)
		{
			GLuint instanceId = instanceIds[i];
			auto resourceIdentifierIter = identifiers.find(instanceId);
			if (resourceIdentifierIter != identifiers.end())
			{
				std::cout << "Resource " << instanceId << " has been requested!" << std::endl;
				continue;
			}
			newInstanceIds.push_back(instanceId);
		}
		return newInstanceIds;
	}

	template<typename Identifier>
	Identifier* getResource(GLuint instanceId, std::unordered_map<GLuint, Identifier>& resourceIdentifier)
	{
		auto resourceIter = resourceIdentifier.find(instanceId);
		if (resourceIter != resourceIdentifier.end())
		{
			return &resourceIdentifier[instanceId];
		}
		return nullptr;
	}
private:
	MeshManagementCentre m_meshManagementCentre;

	TextureManagementCentre m_textureManagementCentre;

	RenderTargetManagementCentre m_renderTargetManagementCentre;
	
	std::unordered_map<GLuint, RenderBuffer> m_renderBuffers;

	std::unordered_map<GLuint, MeshResourceIdentifier> m_meshResources;
	std::unordered_map<GLuint, TextureResourceIdentifier> m_textureResources;
	std::unordered_map<GLuint, SamplerResouceIdentifier> m_samplerResources;
	std::unordered_map<GLuint, RenderTargetIdentifier> m_renderTargetResources;

	std::unordered_map<GLuint, RenderBufferIdentifier> m_renderBufferResources;

	DeviceBase* m_device;
};
