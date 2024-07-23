#include "ResourceManager.h"
void ResourceManager::RequestMeshResource(const MeshManagementCentre& meshManagementCentre, std::vector<GLuint>& meshInstanceIds)
{
	size_t length = meshInstanceIds.size();
	GLuint* newMeshInstanceIds = new GLuint[length];
	GLuint* newMeshVaos;
	GLuint* newMeshVbos;
	GLuint* newMeshEbos;

	GLsizei newMeshCount = 0;
	for (size_t i = 0;i < length;++i)
	{
		GLuint meshInstanceId = meshInstanceIds[i];
		auto meshResourceIdentifierIter = m_meshResources.find(meshInstanceId);
		if (meshResourceIdentifierIter != m_meshResources.end())
		{
			std::cout << "Resource " << meshInstanceId << " has been requested!" << std::endl;
			continue;
		}
		newMeshInstanceIds[newMeshCount++] = meshInstanceId;
	}

	newMeshVaos = new GLuint[newMeshCount];
	newMeshVbos = new GLuint[newMeshCount];
	newMeshEbos = new GLuint[newMeshCount];

	glCreateVertexArrays(newMeshCount, newMeshVaos);
	glCreateBuffers(newMeshCount, newMeshVbos);
	glCreateBuffers(newMeshCount, newMeshEbos);
	for (int i = 0;i < newMeshCount;++i)
	{
		GLuint vao = newMeshVaos[i];
		GLuint vbo = newMeshVbos[i];
		GLuint ebo = newMeshEbos[i];
		GLuint instanceId = newMeshInstanceIds[i];
		auto meshItem = meshManagementCentre.GetRefObject(instanceId);
		auto mesh = meshItem.m_target;
		size_t verticesMemorySize = mesh->GetVerticesCount() * Mesh::VertexSize;
		size_t colorsMemorySize = mesh->GetVerticesCount() * Mesh::ColorSize;
		size_t uvsMemorySize = mesh->GetVerticesCount() * Mesh::UVSize;
		size_t normalsMemorySize = mesh->GetVerticesCount() * Mesh::NormalSize;

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		if (verticesMemorySize > 0)
		{
			// 0号位置属性，4个浮点数组成的向量，指针指向缓存起始位置
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);
			/*glBufferData(GL_ARRAY_BUFFER, verticesMemorySize + colorsMemorySize, NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, verticesMemorySize, mesh->GetVertices());*/
			glNamedBufferStorage(vbo, verticesMemorySize + colorsMemorySize + uvsMemorySize + normalsMemorySize, NULL, GL_DYNAMIC_STORAGE_BIT);
			glNamedBufferSubData(vbo, 0, verticesMemorySize, mesh->GetVertices());
			if (colorsMemorySize > 0)
			{
				auto offset = verticesMemorySize;
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,(void*)offset);
				glEnableVertexAttribArray(1);
				glNamedBufferSubData(vbo, offset, colorsMemorySize, mesh->GetColors());
				//glBufferSubData(GL_ARRAY_BUFFER, verticesMemorySize, colorsMemorySize, mesh->GetColors());
			}
			if (uvsMemorySize > 0)
			{
				auto offset = colorsMemorySize + verticesMemorySize;
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				glEnableVertexAttribArray(2);
				glNamedBufferSubData(vbo, offset, uvsMemorySize, mesh->GetUvs());
			}
			if (normalsMemorySize > 0)
			{
				auto offset = colorsMemorySize + verticesMemorySize + uvsMemorySize;
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				glEnableVertexAttribArray(3);
				glNamedBufferSubData(vbo, offset, normalsMemorySize, mesh->GetNormals());
			}
		}
		glNamedBufferStorage(ebo, mesh->GetIndicesCount() * Mesh::IndexSize, mesh->GetIndices(), GL_DYNAMIC_STORAGE_BIT);

		m_meshResources[instanceId] = MeshResourceIdentifier(vao, vbo, ebo, instanceId);
	}

	delete[] newMeshVaos;
	delete[] newMeshVbos;
	delete[] newMeshEbos;
	delete[] newMeshInstanceIds;
	newMeshVaos = nullptr;
	newMeshVbos = nullptr;
	newMeshEbos = nullptr;
	newMeshInstanceIds = nullptr;
}

void ResourceManager::DestroyMeshResource(const MeshManagementCentre& meshManagementCentre, std::vector<GLuint>& meshInstanceIds)
{
	GLsizei removedCount = (GLsizei)meshInstanceIds.size();
	GLuint* removedVaos = new GLuint[removedCount];
	GLuint* removedVbos = new GLuint[removedCount];
	GLuint* removedEbos = new GLuint[removedCount];
	for (size_t i = 0;i < removedCount;++i)
	{
		auto instanceId = meshInstanceIds[i];
		auto targetIter = m_meshResources.find(instanceId);
		if (targetIter == m_meshResources.end())
		{
			continue;
		}
		auto meshResourceIdentifier = (*targetIter).second;

		removedVaos[i] = meshResourceIdentifier.GetVAO();
		removedVbos[i] = meshResourceIdentifier.GetVBO();
		removedEbos[i] = meshResourceIdentifier.GetEBO();
	}
	glDeleteVertexArrays(removedCount, removedVaos);
	glDeleteBuffers(removedCount, removedVbos);
	glDeleteBuffers(removedCount, removedEbos);
	delete[] removedVaos;
	delete[] removedVbos;
	delete[] removedEbos;
	removedVaos = nullptr;
	removedVbos = nullptr;
	removedEbos = nullptr;
}

void ResourceManager::RequestTextureResource(const TextureManagementCentre& textureManagementCentre, std::vector<GLuint>& textureInstanceIds)
{
	std::vector<GLuint> newTextureInstanceIds = RequestNewIdentifier(textureInstanceIds, m_textureResources);
	size_t newTextureCount = newTextureInstanceIds.size();

	// 收集新的实例
	for (size_t i = 0;i < newTextureCount;++i)
	{
		auto instanceId = newTextureInstanceIds[i];
		TextureResourceIdentifier textureResourceIdentifier = TextureResourceIdentifier(instanceId);
		const auto textureRef = textureManagementCentre.GetRefObject(instanceId);
		auto texturePtr = textureRef.m_target;
		const auto textureType = texturePtr->GetTextureType();
		textureResourceIdentifier.m_levels = texturePtr->GetLevels();
		textureResourceIdentifier.m_internalFormat = texturePtr->GetInternalFormat();
		textureResourceIdentifier.m_textureType = textureType;
		textureResourceIdentifier.m_width = texturePtr->GetWidth();
		if (textureType == TextureType::TextureType_Texture2D)
		{
			auto texture2DPtr = static_cast<Texture2D*>(texturePtr.get());
			textureResourceIdentifier.m_height = texture2DPtr->GetHeight();
		}
		else if (textureType == TextureType::TextureType_Texture3D)
		{
			auto texture3DPtr = static_cast<Texture3D*>(texturePtr.get());
			textureResourceIdentifier.m_height = texture3DPtr->GetHeight();
			textureResourceIdentifier.m_depth = texture3DPtr->GetDepth();
		}
		textureResourceIdentifier.m_perChannelSize = texturePtr->GetPerChannelSize();
		textureResourceIdentifier.m_externalFormat = texturePtr->GetExternalFormat();
		if (m_tempTextureResources.find(textureResourceIdentifier.m_textureType) == m_tempTextureResources.end())
		{
			m_tempTextureResources[textureResourceIdentifier.m_textureType] = std::vector<TextureResourceIdentifier*>();
		}
		m_tempTextureResources[textureResourceIdentifier.m_textureType].push_back(&textureResourceIdentifier);
	}
	// 创建纹理对象
	GLuint* textures;
	for (const auto& pair : m_tempTextureResources)
	{
		auto textureType = pair.first;
		auto textureResourceIdentifiers = pair.second;
		auto size = (GLsizei)textureResourceIdentifiers.size();
		textures = new GLuint[size];
		GLenum target = Texture::TextureType2TextureTarget(textureType);
		if (target == GL_NONE)
		{
			continue;
		}
		glCreateTextures(target, size, textures);
		for (int i = 0;i < size;++i)
		{
			(*textureResourceIdentifiers[i]).m_texture = textures[i];
			//glBindTexture(target, textures[i]);
		}
		delete[] textures;
		textures = nullptr;
	}
	// 分配存储空间
	for (const auto& pair : m_tempTextureResources)
	{
		auto textureType = pair.first;
		auto textureResourceIdentifiers = pair.second;
		for (const auto& resourceIdentifier : textureResourceIdentifiers)
		{
			const auto textureRef = textureManagementCentre.GetRefObject(resourceIdentifier->GetInstanceId());
			auto texturePtr = textureRef.m_target;
			if (textureType == TextureType::TextureType_Texture1D)
			{
				glTextureStorage1D(resourceIdentifier->m_texture, resourceIdentifier->m_levels, resourceIdentifier->m_internalFormat, resourceIdentifier->m_width);
			}
			else if (textureType == TextureType::TextureType_Texture2D)
			{
				auto texture2DPtr = static_cast<Texture2D*>(texturePtr.get());
				glBindTextureUnit(0, resourceIdentifier->m_texture);

				SetTextureWrapMode(resourceIdentifier->m_texture, GL_TEXTURE_WRAP_S, texture2DPtr->GetWrapModeS());
				SetTextureWrapMode(resourceIdentifier->m_texture, GL_TEXTURE_WRAP_T, texture2DPtr->GetWrapModeT());

				SetTextureFilter(resourceIdentifier->m_texture, GL_TEXTURE_MIN_FILTER, texture2DPtr->GetTextureFilter());
				SetTextureFilter(resourceIdentifier->m_texture, GL_TEXTURE_MAG_FILTER, texture2DPtr->GetTextureFilter());

				glTextureStorage2D(resourceIdentifier->m_texture, resourceIdentifier->m_levels, resourceIdentifier->m_internalFormat, resourceIdentifier->m_width, resourceIdentifier->m_height);
				glTextureSubImage2D(resourceIdentifier->m_texture, 0, 0, 0, resourceIdentifier->m_width,
					resourceIdentifier->m_height, resourceIdentifier->m_externalFormat, resourceIdentifier->m_perChannelSize, texturePtr->GetData());

				glGenerateTextureMipmap(resourceIdentifier->m_texture);
			}
			else if (textureType == TextureType::TextureType_Texture3D)
			{
				glTextureStorage3D(resourceIdentifier->m_texture, resourceIdentifier->m_levels, resourceIdentifier->m_internalFormat, resourceIdentifier->m_width, resourceIdentifier->m_height,
					resourceIdentifier->m_depth);
			}
			// Todo其他类型
		}
	}
	// 保存
	for (const auto& pair : m_tempTextureResources)
	{
		auto textureType = pair.first;
		auto textureResourceIdentifiers = pair.second;
		for (const auto& resourceIdentifier : textureResourceIdentifiers)
		{
			m_textureResources[resourceIdentifier->GetInstanceId()] = *resourceIdentifier;
		}
	}
	m_tempTextureResources.clear();
}

void ResourceManager::DestroyTextureResource(const TextureManagementCentre& textureManagementCentre, std::vector<GLuint>& textureInstanceIds)
{
	size_t length = textureInstanceIds.size();
	std::vector<GLuint> removedTextures;
	for (size_t i = 0;i < length;++i)
	{
		auto instanceId = textureInstanceIds[i];
		auto targetIter = m_textureResources.find(instanceId);
		if (targetIter == m_textureResources.end())
		{
			continue;
		}
		auto textureResourceIdentifier = (*targetIter).second;

		removedTextures.push_back(textureResourceIdentifier.m_texture);
	}
	glDeleteTextures((GLsizei)removedTextures.size(), removedTextures.data());
}

void ResourceManager::RequestSamplerResource(const SamplerManagementCentre& samplerManagementCentre, std::vector<GLuint>& samplerInstanceIds)
{
	std::vector<GLuint> newSamplerInstanceIds = RequestNewIdentifier(samplerInstanceIds, m_samplerResources);
	GLsizei newSamplerCount = (GLsizei)newSamplerInstanceIds.size();
	for (int i = 0;i < newSamplerCount;++i)
	{
		SamplerResouceIdentifier samplerResourceIdentifier;
		const auto samplerRef = samplerManagementCentre.GetRefObject(newSamplerInstanceIds[i]);
		auto samplerPtr = samplerRef.m_target;
		// Sampler -> SamplerResourceIdentifier 参数设置

		m_samplerResources[newSamplerInstanceIds[i]] = samplerResourceIdentifier;
	}

	// 创建纹理对象
	std::vector<GLuint> samplers = std::vector<GLuint>(newSamplerCount);
	glCreateSamplers(newSamplerCount, samplers.data());
	for (size_t i = 0;i < newSamplerCount;++i)
	{
		auto instanceId = newSamplerInstanceIds[i];
		auto textureResourceIdentifier = m_samplerResources[instanceId];
		textureResourceIdentifier.m_sampler = samplers[i];
	}
}

void ResourceManager::DestroySamplerResource(const SamplerManagementCentre& samplerManagementCentre, std::vector<GLuint>& samplerInstanceIds)
{
	size_t length = samplerInstanceIds.size();
	std::vector<GLuint> removedSamplers;
	for (size_t i = 0;i < length;++i)
	{
		auto instanceId = samplerInstanceIds[i];
		auto targetIter = m_samplerResources.find(instanceId);
		if (targetIter == m_samplerResources.end())
		{
			continue;
		}
		auto samplerResourceIdentifier = (*targetIter).second;

		removedSamplers.push_back(samplerResourceIdentifier.m_sampler);
	}
	glDeleteSamplers((GLsizei)removedSamplers.size(), removedSamplers.data());
}

