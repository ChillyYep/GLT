#pragma once
#include <CommonDefine.h>
#include <Texture.h>
#include <ResourceIdentifier.h>
#include <ConstantBufferIdentifier.h>
#include <MeshManagementCentre.h>
#include <TextureManagementCentre.h>
#include <SamplerManagementCentre.h>

enum ResourceType {
	ResourceType_Mesh,
	ResourceType_Texture,
	ResourceType_Sampler
};

class ResourceManager
{
public:
	~ResourceManager() {}
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	inline static ResourceManager& GetInstance() {

		static ResourceManager instance;
		return instance;
	}
	template<typename T>
	inline void RequestResource(ManagementCentreBase<T>* managementCentre, std::vector<GLuint> instanceIds, ResourceType resourceType)
	{
		switch (resourceType)
		{
		case ResourceType_Mesh:
			RequestMeshResource(*dynamic_cast<MeshManagementCentre*>(managementCentre), instanceIds);
			break;
		case ResourceType_Texture:
			RequestTextureResource(*dynamic_cast<TextureManagementCentre*>(managementCentre), instanceIds);
			break;
		case ResourceType_Sampler:
			RequestSamplerResource(*dynamic_cast<SamplerManagementCentre*>(managementCentre), instanceIds);
			break;
		default:
			break;
		}
	}

	template<typename T>
	inline void DestroyResource(ManagementCentreBase<T>* managementCentre, std::vector<GLuint> instanceIds, ResourceType resourceType)
	{
		switch (resourceType)
		{
		case ResourceType_Mesh:
			DestroyMeshResource(*dynamic_cast<MeshManagementCentre*>(managementCentre), instanceIds);
			break;
		case ResourceType_Texture:
			DestroyTextureResource(*dynamic_cast<TextureManagementCentre*>(managementCentre), instanceIds);
			break;
		case ResourceType_Sampler:
			DestroySamplerResource(*dynamic_cast<SamplerManagementCentre*>(managementCentre), instanceIds);
			break;
		default:
			break;
		}
	}

	void RequestMeshResource(const MeshManagementCentre& meshManagementCentre, std::vector<GLuint>& meshInstanceIds);

	void DestroyMeshResource(const MeshManagementCentre& meshManagementCentre, std::vector<GLuint>& meshInstanceIds);

	void RequestTextureResource(const TextureManagementCentre& textureManagementCentre, std::vector<GLuint>& textureInstanceIds);

	void DestroyTextureResource(const TextureManagementCentre& textureManagementCentre, std::vector<GLuint>& textureInstanceIds);

	void RequestSamplerResource(const SamplerManagementCentre& samplerManagementCentre, std::vector<GLuint>& samplerInstanceIds);

	void DestroySamplerResource(const SamplerManagementCentre& samplerManagementCentre, std::vector<GLuint>& samplerInstanceIds);

	void RequestConstantBufferResource(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers)
	{
		int size = (int)constantBufferIdentifiers.size();
		std::vector<GLuint> ubos = std::vector<GLuint>(size);
		glCreateBuffers(size, ubos.data());
		for (int i = 0;i < size;++i)
		{
			constantBufferIdentifiers[i].SetUbo(ubos[i]);
			size_t bufferSize = constantBufferIdentifiers[i].GetTotalBufferSize();
			// ·ÖÅä´æ´¢¿Õ¼ä
			glBindBuffer(GL_UNIFORM_BUFFER, ubos[i]);
			glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
			//glNamedBufferStorage(ubo, bufferSize, NULL, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	}

	inline MeshResourceIdentifier GetMeshResource(GLuint meshInstanceId)
	{
		return GetResource(meshInstanceId, m_meshResources);
	}

	inline TextureResourceIdentifier GetTextureResource(GLuint textureInstanceId)
	{
		return GetResource(textureInstanceId, m_textureResources);
	}

	inline MeshManagementCentre& GetMeshManagementCentre() { return m_meshManagementCentre; }

	inline TextureManagementCentre& GetTextureManagementCentre() { return m_textureManagementCentre; }
private:
	ResourceManager() {}

	template<typename Identifier>
	std::vector<GLuint> RequestNewIdentifier(std::vector<GLuint>& instanceIds, std::unordered_map<GLuint, Identifier>& identifiers)
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
	Identifier GetResource(GLuint instanceId, std::unordered_map<GLuint, Identifier>& resourceIdentifier)
	{
		auto resource = resourceIdentifier.find(instanceId);
		if (resource != resourceIdentifier.end())
		{
			return (*resource).second;
		}
		return Identifier();
	}

	inline void SetTextureWrapMode(GLuint texture, GLenum pname, TextureWrapMode wrapMode)
	{
		switch (wrapMode)
		{
		case TextureWrapMode::TextureWrapMode_ClampEdge:
			glTextureParameteri(texture, pname, GL_CLAMP_TO_EDGE);
			break;
		case TextureWrapMode::TextureWrapMode_Border:
			glTextureParameteri(texture, pname, GL_CLAMP_TO_BORDER);
			break;
		case TextureWrapMode::TextureWrapMode_Repeat:
			glTextureParameteri(texture, pname, GL_REPEAT);
			break;
		case TextureWrapMode::TextureWrapMode_MirroredRepeat:
			glTextureParameteri(texture, pname, GL_MIRRORED_REPEAT);
			break;
		default:
			break;
		}
	}

	inline void SetTextureFilter(GLuint texture, GLenum pname, TextureFilter textureFilter)
	{
		switch (textureFilter)
		{
		case TextureFilter::TextureFilter_Point_Mipmap_Point:
			glTextureParameteri(texture, pname, GL_NEAREST_MIPMAP_NEAREST);
			break;
		case TextureFilter::TextureFilter_Point_Mipmap_Linear:
			glTextureParameteri(texture, pname, GL_NEAREST_MIPMAP_LINEAR);
			break;
		case TextureFilter::TextureFilter_Linear_Mipmap_Point:
			glTextureParameteri(texture, pname, GL_LINEAR_MIPMAP_NEAREST);
			break;
		case TextureFilter::TextureFilter_Linear_Mipmap_Linear:
			glTextureParameteri(texture, pname, GL_LINEAR_MIPMAP_LINEAR);
			break;
		default:
			break;
		}
	}

private:
	MeshManagementCentre m_meshManagementCentre;

	TextureManagementCentre m_textureManagementCentre;

	std::unordered_map<TextureType, std::vector<TextureResourceIdentifier*>> m_tempTextureResources;
	std::unordered_map<GLuint, MeshResourceIdentifier> m_meshResources;
	std::unordered_map<GLuint, TextureResourceIdentifier> m_textureResources;
	std::unordered_map<GLuint, SamplerResouceIdentifier> m_samplerResources;

};
