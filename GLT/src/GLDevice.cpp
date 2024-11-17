#include "GLDevice.h"

std::vector<MeshResourceIdentifier> GLDevice::requestMeshResources(std::vector<SubMesh*>& meshPtrs)
{
	GLsizei length = (GLsizei)meshPtrs.size();
	std::vector<MeshResourceIdentifier> meshResourceIdentifiers = std::vector<MeshResourceIdentifier>(length);
	std::vector<GLuint> newMeshVaos = std::vector<GLuint>(length);
	std::vector<GLuint> newMeshVbos = std::vector<GLuint>(length);
	std::vector<GLuint> newMeshEbos = std::vector<GLuint>(length);
	// 创建VAO，VBO，EBO对象
	glCreateVertexArrays(length, newMeshVaos.data());
	glCreateBuffers(length, newMeshVbos.data());
	glCreateBuffers(length, newMeshEbos.data());
	// 申请存储空间，并设置Mesh相关属性
	for (int i = 0; i < length; ++i)
	{
		GLuint vao = newMeshVaos[i];
		GLuint vbo = newMeshVbos[i];
		GLuint ebo = newMeshEbos[i];
		auto mesh = meshPtrs[i];
		GLuint instanceId = mesh->getInstanceId();
		size_t verticesMemorySize = mesh->getVerticesCount() * SubMesh::VertexSize;
		size_t colorsMemorySize = mesh->getColors() == nullptr ? 0 : mesh->getVerticesCount() * SubMesh::ColorSize;
		size_t uvsMemorySize = mesh->getUvs() == nullptr ? 0 : mesh->getVerticesCount() * SubMesh::UVSize;
		size_t normalsMemorySize = mesh->getNormals() == nullptr ? 0 : mesh->getVerticesCount() * SubMesh::NormalSize;

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		if (verticesMemorySize > 0)
		{
			// 0号位置属性，4个浮点数组成的向量，指针指向缓存起始位置
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);

			glNamedBufferStorage(vbo, verticesMemorySize + colorsMemorySize + uvsMemorySize + normalsMemorySize, NULL, GL_DYNAMIC_STORAGE_BIT);
			if (colorsMemorySize > 0)
			{
				auto offset = verticesMemorySize;
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				glEnableVertexAttribArray(1);
			}
			else {
				glDisableVertexAttribArray(1);
			}
			if (uvsMemorySize > 0)
			{
				auto offset = colorsMemorySize + verticesMemorySize;
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				glEnableVertexAttribArray(2);
			}
			else {
				glDisableVertexAttribArray(2);
			}
			if (normalsMemorySize > 0)
			{
				auto offset = colorsMemorySize + verticesMemorySize + uvsMemorySize;
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				glEnableVertexAttribArray(3);
			}
			else {
				glDisableVertexAttribArray(3);
			}
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glNamedBufferStorage(ebo, mesh->getIndicesCount() * SubMesh::IndexSize, mesh->getIndices(), GL_DYNAMIC_STORAGE_BIT);

		meshResourceIdentifiers[i] = MeshResourceIdentifier(vao, vbo, ebo, instanceId, mesh->getVerticesCount(), mesh->getIndicesCount());
	}
	updateMeshResources(meshPtrs, meshResourceIdentifiers);
	return meshResourceIdentifiers;
}

void GLDevice::updateMeshResources(std::vector<SubMesh*>& meshPtrs, std::vector<MeshResourceIdentifier>& meshResourceIdentifiers)
{
	GLsizei length = (GLsizei)meshPtrs.size();

	for (int i = 0; i < length; ++i)
	{
		auto mesh = meshPtrs[i];
		auto meshResourceIdentifier = meshResourceIdentifiers[i];

		GLuint vao = meshResourceIdentifier.getVAO();
		GLuint vbo = meshResourceIdentifier.getVBO();
		GLuint ebo = meshResourceIdentifier.getEBO();

		GLuint instanceId = mesh->getInstanceId();
		size_t verticesMemorySize = mesh->getVerticesCount() * SubMesh::VertexSize;
		size_t colorsMemorySize = mesh->getColors() == nullptr ? 0 : mesh->getVerticesCount() * SubMesh::ColorSize;
		size_t uvsMemorySize = mesh->getUvs() == nullptr ? 0 : mesh->getVerticesCount() * SubMesh::UVSize;
		size_t normalsMemorySize = mesh->getNormals() == nullptr ? 0 : mesh->getVerticesCount() * SubMesh::NormalSize;

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		if (verticesMemorySize > 0)
		{
			glNamedBufferSubData(vbo, 0, verticesMemorySize, mesh->getVertices());
			if (colorsMemorySize > 0)
			{
				auto offset = verticesMemorySize;
				glNamedBufferSubData(vbo, offset, colorsMemorySize, mesh->getColors());
			}
			if (uvsMemorySize > 0)
			{
				auto offset = colorsMemorySize + verticesMemorySize;
				glNamedBufferSubData(vbo, offset, uvsMemorySize, mesh->getUvs());
			}
			if (normalsMemorySize > 0)
			{
				auto offset = colorsMemorySize + verticesMemorySize + uvsMemorySize;
				glNamedBufferSubData(vbo, offset, normalsMemorySize, mesh->getNormals());
			}
		}
	}
}


void GLDevice::destroyMeshResources(std::vector<MeshResourceIdentifier>& meshIdentifiers)
{
	GLsizei removedCount = (GLsizei)meshIdentifiers.size();
	std::vector<GLuint> removedVaos = std::vector<GLuint>(removedCount);
	std::vector<GLuint> removedVbos = std::vector<GLuint>(removedCount);
	std::vector<GLuint> removedEbos = std::vector<GLuint>(removedCount);
	for (size_t i = 0; i < removedCount; ++i)
	{
		removedVaos[i] = meshIdentifiers[i].getVAO();
		removedVbos[i] = meshIdentifiers[i].getVBO();
		removedEbos[i] = meshIdentifiers[i].getEBO();
	}
	glDeleteVertexArrays(removedCount, removedVaos.data());
	glDeleteBuffers(removedCount, removedVbos.data());
	glDeleteBuffers(removedCount, removedEbos.data());
}

std::vector<TextureResourceIdentifier> GLDevice::requestTextureResources(std::vector<Texture*>& texturePtrs)
{
	size_t newTextureCount = texturePtrs.size();
	std::vector<TextureResourceIdentifier> textureResourceIdentifiers = std::vector<TextureResourceIdentifier>(newTextureCount);
	std::map<TextureType, std::vector<int>> textureMap;

	// 纹理分类，方便批量创建纹理对象
	for (int i = 0; i < newTextureCount; ++i)
	{
		auto textureType = texturePtrs[i]->getTextureType();;
		if (textureMap.find(textureType) == textureMap.end())
		{
			textureMap[textureType] = std::vector<int>();
		}
		textureMap[textureType].push_back(i);
	}

	// 批量创建纹理对象及标识符
	for (const auto& texturePair : textureMap)
	{
		auto textureType = texturePair.first;
		auto textureIndices = texturePair.second;
		GLenum target = textureType2TextureTarget(textureType);
		if (target == GL_NONE)
		{
			continue;
		}
		GLsizei size = (GLsizei)textureIndices.size();
		// 创建N个纹理对象，并生成标识符
		std::vector<GLuint> tempTextures = std::vector<GLuint>(size);
		if (size > 0)
		{
			glCreateTextures(target, size, tempTextures.data());
			for (int i = 0; i < size; ++i)
			{
				int index = textureIndices[i];
				auto texturePtr = texturePtrs[index];
				auto identifier = TextureResourceIdentifier(texturePtr->getInstanceId());
				identifier.m_levels = texturePtr->getLevels();
				identifier.m_internalFormat = texturePtr->getInternalFormat();
				identifier.m_textureType = textureType;
				identifier.m_width = texturePtr->getWidth();
				if (textureType == TextureType::Texture2D)
				{
					auto texture2DPtr = static_cast<Texture2D*>(texturePtr);
					identifier.m_height = texture2DPtr->getHeight();
				}
				else if (textureType == TextureType::Texture3D)
				{
					auto texture3DPtr = static_cast<Texture3D*>(texturePtr);
					identifier.m_height = texture3DPtr->getHeight();
					identifier.m_depth = texture3DPtr->getDepth();
				}
				else if (textureType == TextureType::CubeMap)
				{
					// todo
					auto cubemapPtr = static_cast<Cubemap*>(texturePtr);
					identifier.m_height = cubemapPtr->getHeight();
				}
				identifier.m_perChannelSize = texturePtr->getPerChannelSize();
				identifier.m_externalFormat = texturePtr->getExternalFormat();

				identifier.m_texture = tempTextures[i];
				textureResourceIdentifiers[index] = identifier;
			}
		}
	}

	// 分配存储空间，并设置纹理基础信息
	for (int i = 0; i < newTextureCount; ++i)
	{
		const auto& texturePtr = texturePtrs[i];
		auto& resourceIdentifier = textureResourceIdentifiers[i];
		auto textureType = resourceIdentifier.m_textureType;
		auto internalFormat = getGLTextureInternalFormat(resourceIdentifier.m_internalFormat);
		auto externalFormat = getGLTextureExternalFormat(resourceIdentifier.m_externalFormat);
		auto perChannelSize = getGLTextureChannelSize(resourceIdentifier.m_perChannelSize);
		if (textureType == TextureType::Texture1D)
		{
			glTextureStorage1D(resourceIdentifier.m_texture, resourceIdentifier.m_levels, internalFormat, resourceIdentifier.m_width);
		}
		else if (textureType == TextureType::Texture2D)
		{
			auto texture2DPtr = static_cast<Texture2D*>(texturePtr);
			//glBindTextureUnit(0, resourceIdentifier.m_texture);

			glTextureParameteri(resourceIdentifier.m_texture, GL_TEXTURE_WRAP_S, getGLTextureWrapMode(texture2DPtr->getWrapModeS()));
			glTextureParameteri(resourceIdentifier.m_texture, GL_TEXTURE_WRAP_T, getGLTextureWrapMode(texture2DPtr->getWrapModeT()));
			glTextureParameterfv(resourceIdentifier.m_texture, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(texture2DPtr->getBorderColor()));
			glTextureParameteri(resourceIdentifier.m_texture, GL_TEXTURE_MIN_FILTER, getGLTextureFilter(texture2DPtr->getTextureFilter()));
			glTextureParameteri(resourceIdentifier.m_texture, GL_TEXTURE_MAG_FILTER, getGLTextureFilter(texture2DPtr->getTextureFilter()));
			/*	if (texture2DPtr->IsDepthTexture())
				{
					glTextureParameteri(resourceIdentifier.m_texture, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
					glTextureParameteri(resourceIdentifier.m_texture, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
				}*/
				//glTextureParameteri(resourceIdentifier.m_texture,GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_COMPONENTS);

			glTextureStorage2D(resourceIdentifier.m_texture, resourceIdentifier.m_levels, internalFormat, resourceIdentifier.m_width, resourceIdentifier.m_height);
		}
		else if (textureType == TextureType::Texture3D)
		{
			glTextureStorage3D(resourceIdentifier.m_texture, resourceIdentifier.m_levels, internalFormat, resourceIdentifier.m_width, resourceIdentifier.m_height,
				resourceIdentifier.m_depth);
		}
		else if (textureType == TextureType::CubeMap)
		{
			auto cubemap = static_cast<Cubemap*>(texturePtr);
			glBindTexture(GL_TEXTURE_CUBE_MAP, resourceIdentifier.m_texture);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, getGLTextureFilter(cubemap->getTextureFilter()));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, getGLTextureFilter(cubemap->getTextureFilter()));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, getGLTextureWrapMode(cubemap->getWrapModeS()));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, getGLTextureWrapMode(cubemap->getWrapModeT()));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, getGLTextureWrapMode(cubemap->getWrapModeR()));
			glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(cubemap->getBorderColor()));
			for (int i = 0; i < Cubemap::CUBEMAP_FACENUM; ++i)
			{
				auto tex2D = cubemap->getFace((CubemapFace)(i));
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, resourceIdentifier.m_width, resourceIdentifier.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			}
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		// Todo其他类型
	}

	// 更新纹理数据
	updateTextureResources(texturePtrs, textureResourceIdentifiers);

	return textureResourceIdentifiers;
}

void GLDevice::updateTextureResources(std::vector<Texture*>& texturePtrs, std::vector<TextureResourceIdentifier>& textureResourceIdentifiers)
{
	GLsizei newTextureCount = (GLsizei)texturePtrs.size();
	// 分配存储空间
	for (int i = 0; i < newTextureCount; ++i)
	{
		const auto& texturePtr = texturePtrs[i];
		auto resourceIdentifier = textureResourceIdentifiers[i];
		auto textureType = resourceIdentifier.m_textureType;
		auto externalFormat = getGLTextureExternalFormat(resourceIdentifier.m_externalFormat);
		auto perChannelSize = getGLTextureChannelSize(resourceIdentifier.m_perChannelSize);
		if (textureType == TextureType::Texture1D)
		{
		}
		else if (textureType == TextureType::Texture2D)
		{
			auto texture2DPtr = static_cast<Texture2D*>(texturePtr);

			auto data = texturePtr->getData();
			if (data != nullptr)
			{
				glTextureSubImage2D(resourceIdentifier.m_texture, 0, 0, 0, resourceIdentifier.m_width,
					resourceIdentifier.m_height, externalFormat, perChannelSize, texturePtr->getData());
			}

			glGenerateTextureMipmap(resourceIdentifier.m_texture);
		}
		else if (textureType == TextureType::Texture3D)
		{
		}
		else if (textureType == TextureType::CubeMap)
		{
			auto cubemapPtr = static_cast<Cubemap*>(texturePtr);
			glBindTexture(GL_TEXTURE_CUBE_MAP, resourceIdentifier.m_texture);
			for (int i = 0; i < Cubemap::CUBEMAP_FACENUM; ++i)
			{
				auto tex2D = cubemapPtr->getFace((CubemapFace)(i));
				auto texData = tex2D->getData();
				if (texData != nullptr)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, getGLTextureInternalFormat(resourceIdentifier.m_internalFormat), resourceIdentifier.m_width, resourceIdentifier.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
				}
			}
			glGenerateTextureMipmap(resourceIdentifier.m_texture);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		// Todo其他类型
	}

}

void GLDevice::destroyTextureResources(std::vector<TextureResourceIdentifier>& textureIdentifiers)
{
	size_t length = textureIdentifiers.size();
	std::vector<GLuint> removedTextures = std::vector<GLuint>(length);
	for (size_t i = 0; i < length; ++i)
	{
		removedTextures[i] = textureIdentifiers[i].m_texture;
	}
	glDeleteTextures((GLsizei)removedTextures.size(), removedTextures.data());
}

std::vector<SamplerResouceIdentifier> GLDevice::requestSamplerResources(std::vector<Sampler*>& samplerPtrs)
{
	GLsizei newSamplerCount = (GLsizei)samplerPtrs.size();
	std::vector<SamplerResouceIdentifier> samplerResourceIdentifiers = std::vector<SamplerResouceIdentifier>(newSamplerCount);
	for (int i = 0; i < newSamplerCount; ++i)
	{
		SamplerResouceIdentifier samplerResourceIdentifier(samplerPtrs[i]->getInstanceId());
		// Sampler -> SamplerResourceIdentifier 参数设置

		samplerResourceIdentifiers[i] = samplerResourceIdentifier;
	}

	// 创建纹理对象
	std::vector<GLuint> samplers = std::vector<GLuint>(newSamplerCount);
	glCreateSamplers(newSamplerCount, samplers.data());
	for (size_t i = 0; i < newSamplerCount; ++i)
	{
		auto samplerResourceIdentifier = samplerResourceIdentifiers[i];
		samplerResourceIdentifier.m_sampler = samplers[i];
	}

	return samplerResourceIdentifiers;
}

void GLDevice::destroySamplerResources(std::vector<SamplerResouceIdentifier>& samplerIdentifiers)
{
	size_t length = samplerIdentifiers.size();
	std::vector<GLuint> removedSamplers = std::vector<GLuint>(length);
	for (size_t i = 0; i < length; ++i)
	{
		removedSamplers[i] = samplerIdentifiers[i].m_sampler;
	}
	glDeleteSamplers((GLsizei)removedSamplers.size(), removedSamplers.data());
}

std::vector<RenderTargetIdentifier> GLDevice::requestRenderTargetResource(std::vector<RenderTarget*>& renderTargetPtrs, std::vector<std::vector<AttachmentEntityIdentifierWrapper>>& rtsAttachmentIdentifiers)
{
	GLsizei newRenderTextureCount = (GLsizei)renderTargetPtrs.size();
	std::vector<RenderTargetIdentifier> renderTargetResources = std::vector<RenderTargetIdentifier>(newRenderTextureCount);
	if (renderTargetPtrs.size() != rtsAttachmentIdentifiers.size())
	{
		return renderTargetResources;
	}

	// 创建RT对象
	std::vector<GLuint> rts = std::vector<GLuint>(newRenderTextureCount);
	glCreateFramebuffers(newRenderTextureCount, rts.data());
	for (size_t i = 0; i < newRenderTextureCount; ++i)
	{
		auto rtPtr = renderTargetPtrs[i];
		renderTargetResources[i] = RenderTargetIdentifier(rtPtr->getInstanceId());
		auto& m_colorRTIdentifier = renderTargetResources[i];
		auto fbo = rts[i];
		m_colorRTIdentifier.m_fbo = fbo;
		m_colorRTIdentifier.m_descriptor = rtPtr->getRenderTargetDescriptor();
		auto attachmentIdentifiers = rtsAttachmentIdentifiers[i];
		auto descriptor = rtPtr->getRenderTargetDescriptor();

		m_colorRTIdentifier.m_attachmentIdentifiers = attachmentIdentifiers;

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		for (const auto& attachment : attachmentIdentifiers)
		{
			auto resourceType = attachment.getResourceType();
			auto attachmentType = attachment.getAttachmentType();
			if (resourceType == FBOAttachmentResourceType::RenderBuffer)
			{
				auto identifier = attachment.getRenderBufferIdentifier();
				auto renderBuffer = identifier->m_renderBuffer;
				if (!identifier->m_isDepthBuffer)
				{
					if (attachmentType == FBOAttachmentType::Color)
					{
						glNamedFramebufferRenderbuffer(fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
					}
				}
				else
				{
					if (attachmentType == FBOAttachmentType::DepthStencil)
					{
						glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
					}
					else if (attachmentType == FBOAttachmentType::Depth)
					{
						glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
					}
					else
					{
						glNamedFramebufferRenderbuffer(fbo, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
					}
				}
			}
			else if (resourceType == FBOAttachmentResourceType::Texture)
			{
				auto identifier = attachment.getTextureIdentifier();
				auto texture = identifier->m_texture;
				if (attachmentType == FBOAttachmentType::Color)
				{
					glBindTexture(GL_TEXTURE_2D, texture);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				else if (attachmentType == FBOAttachmentType::DepthStencil)
				{
					glBindTexture(GL_TEXTURE_2D, texture);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				else if (attachmentType == FBOAttachmentType::Depth)
				{
					glBindTexture(GL_TEXTURE_2D, texture);
					// 设置textureProj采样时的比较模式
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, texture);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	return renderTargetResources;
}

void GLDevice::destroyRenderTargetResource(std::vector<RenderTargetIdentifier>& renderTargetIdentifiers)
{
	size_t length = renderTargetIdentifiers.size();
	std::vector<GLuint> removedRTs(length);
	for (size_t i = 0; i < length; ++i)
	{
		removedRTs[i] = renderTargetIdentifiers[i].m_fbo;
	}
	glDeleteFramebuffers((GLsizei)removedRTs.size(), removedRTs.data());
}

std::vector<RenderBufferIdentifier> GLDevice::requestRenderBufferResources(std::vector<RenderBuffer*>& renderBufferPtrs)
{
	std::vector<GLuint> renderBufferIds = std::vector<GLuint>(renderBufferPtrs.size());
	std::vector<RenderBufferIdentifier> identifiers = std::vector<RenderBufferIdentifier>(renderBufferPtrs.size());
	// 创建RenderBuffer
	glCreateRenderbuffers((GLsizei)renderBufferPtrs.size(), renderBufferIds.data());
	for (int i = 0; i < renderBufferPtrs.size(); ++i)
	{
		RenderBufferIdentifier identifier(renderBufferPtrs[i]->getInstanceId());
		identifier.m_renderBuffer = renderBufferIds[i];
		identifier.m_isDepthBuffer = renderBufferPtrs[i]->IsDepthBuffer();
		if (identifier.m_isDepthBuffer)
		{
			if (renderBufferPtrs[i]->getInternalFormat() != TextureInternalFormat::None)
			{
				identifier.m_internalFormat = renderBufferPtrs[i]->getInternalFormat();
				identifier.m_width = renderBufferPtrs[i]->getWidth();
				identifier.m_height = renderBufferPtrs[i]->getHeight();
				// 分配RenderBuffer存储空间
				glNamedRenderbufferStorage(renderBufferIds[i], getGLTextureInternalFormat(identifier.m_internalFormat), identifier.m_width, identifier.m_height);
			}
		}
		else {
			if (renderBufferPtrs[i]->getInternalFormat() != TextureInternalFormat::None)
			{
				identifier.m_internalFormat = renderBufferPtrs[i]->getInternalFormat();
				identifier.m_width = renderBufferPtrs[i]->getWidth();
				identifier.m_height = renderBufferPtrs[i]->getHeight();
				// 分配RenderBuffer存储空间
				glNamedRenderbufferStorage(renderBufferIds[i], getGLTextureInternalFormat(identifier.m_internalFormat), identifier.m_width, identifier.m_height);
			}
		}

		identifiers[i] = identifier;
	}
	return identifiers;
}

void GLDevice::destroyRenderBufferResources(std::vector<RenderBufferIdentifier>& renderBufferIdentifiers)
{
	std::vector<GLuint> renderBufferIds = std::vector<GLuint>(renderBufferIdentifiers.size());
	for (int i = 0; i < renderBufferIdentifiers.size(); ++i)
	{
		renderBufferIds[i] = renderBufferIdentifiers[i].m_renderBuffer;
	}
	glDeleteRenderbuffers((GLsizei)renderBufferIds.size(), renderBufferIds.data());
}

void GLDevice::requestConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers)
{
	int size = (int)constantBufferIdentifiers.size();
	std::vector<GLuint> ubos = std::vector<GLuint>(size);
	glCreateBuffers(size, ubos.data());
	for (int i = 0; i < size; ++i)
	{
		constantBufferIdentifiers[i].setUbo(ubos[i]);
		size_t bufferSize = constantBufferIdentifiers[i].getTotalBufferSize();
		// 分配存储空间
		glBindBuffer(GL_UNIFORM_BUFFER, ubos[i]);
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
		//glNamedBufferStorage(ubo, bufferSize, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void GLDevice::destroyConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers)
{
	int size = (int)constantBufferIdentifiers.size();
	std::vector<GLuint> ubos = std::vector<GLuint>(size);
	for (int i = 0; i < size; ++i)
	{
		ubos[i] = constantBufferIdentifiers[i].getUbo();
	}
	glDeleteBuffers(size, ubos.data());
}

void GLDevice::activate(RenderTargetIdentifier* m_colorRTIdentifier)
{
	// 同一RT不必重复设置，减少指令提交
	if (m_colorRTIdentifier == nullptr || (m_curRT != nullptr && m_curRT->getInstanceId() == m_colorRTIdentifier->getInstanceId()))
	{
		return;
	}
	if (m_colorRTIdentifier->m_fbo > 0)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_colorRTIdentifier->m_fbo);
		m_curRT = m_colorRTIdentifier;
	}
}

void GLDevice::clearColor(float r, float g, float b, float a)
{
	//auto size = Window::getInstance()->getSize();
	//glViewport(0, 0, size.x, size.y);
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLDevice::blitCurrentRTToWindow()
{
	blitRTToWindow(m_curRT);
}

void GLDevice::blitRTToWindow(RenderTargetIdentifier* rt)
{
	if (rt == nullptr)
	{
		return;
	}
	int fboAttachmentType = 0;
	for (const auto& attachmentIdentifier : rt->m_attachmentIdentifiers)
	{
		fboAttachmentType |= (int)attachmentIdentifier.getAttachmentType();
	}
	blitRT(rt, nullptr, (FBOAttachmentType)fboAttachmentType);
}
void GLDevice::blitRT(RenderTargetIdentifier* src, RenderTargetIdentifier* dst, FBOAttachmentType attachmentType)
{
	if (src == nullptr)
	{
		return;
	}

	auto windowSize = Window::getInstance()->getSize();

	auto srcWidth = src->m_descriptor.m_width;
	auto srcHeight = src->m_descriptor.m_height;

	auto dstWidth = windowSize.x;
	auto dstHeight = windowSize.y;
	bool isBlitToWindow = dst == nullptr;

	if (!isBlitToWindow)
	{
		dstWidth = dst->m_descriptor.m_width;
		dstHeight = dst->m_descriptor.m_height;
	}

	if (src != nullptr)
	{
		//assert(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		// 将FBO绑定到读取FB目标上
		glBindFramebuffer(GL_READ_FRAMEBUFFER, src->m_fbo);
		// 绘制窗口设置为0，意思是重新绑定到窗口的帧缓存上
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, isBlitToWindow ? 0 : dst->m_fbo);
		// 传到窗口FBO忽略深度/模板缓冲
		if (((int)attachmentType & (int)FBOAttachmentType::Color) > 0)
		{
			glBlitFramebuffer(0, 0, srcWidth, srcHeight, 0, 0, dstWidth, dstHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		}
		if (!isBlitToWindow)
		{
			if (((int)attachmentType & (int)FBOAttachmentType::DepthStencil) == (int)FBOAttachmentType::DepthStencil)
			{
				glBlitFramebuffer(0, 0, srcWidth, srcHeight, 0, 0, dstWidth, dstHeight, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_LINEAR);
			}
			if (((int)attachmentType & (int)FBOAttachmentType::Depth) > 0)
			{
				glBlitFramebuffer(0, 0, srcWidth, srcHeight, 0, 0, dstWidth, dstHeight, GL_DEPTH_BUFFER_BIT, GL_LINEAR);
			}
			if (((int)attachmentType & (int)FBOAttachmentType::Stencil) > 0)
			{
				glBlitFramebuffer(0, 0, srcWidth, srcHeight, 0, 0, dstWidth, dstHeight, GL_STENCIL_BUFFER_BIT, GL_LINEAR);
			}
		}

	}
}

void GLDevice::blitDebugRTToWindow()
{
	//blitRTToWindow(m_debugRT);
}

void GLDevice::bindBlockForProgram(PipelineStateObject& pso)
{
	ConstantBufferSet& globalBuffer = Shader::getShaderConstantBufferSet();
	std::vector<ShaderUniformBlockReference>& blockRefs = pso.m_uniformBlockRefs;
	auto program = pso.m_program;
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, globalBufferIdentifier.getUbo());
	for (int i = 0; i < blockRefs.size(); ++i)
	{
		auto blockIndex = blockRefs[i].m_blockIndex;
		ShaderUniformBlockProperty* block;

		ConstantBufferIdentifier* globalBufferIdentifier = globalBuffer.getGlobalBufferIdentifierByBlockName(blockRefs[i].m_uniformBlockName);
		if (globalBufferIdentifier->findBlock(blockRefs[i].m_uniformBlockName, block))
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, block->m_blockBindingNum, globalBufferIdentifier->getUbo(), block->m_blockOffset, block->m_preDefineSize);
			glUniformBlockBinding(program, blockIndex, block->m_blockBindingNum);
		}
	}
}

GLenum GLDevice::textureType2TextureTarget(TextureType textureType) const
{
	switch (textureType)
	{
	case TextureType::Texture1D:
		return GL_TEXTURE_1D;
	case TextureType::Texture1DArray:
		return GL_TEXTURE_1D_ARRAY;
	case TextureType::Texture2D:
		return GL_TEXTURE_2D;
	case TextureType::Texture2DArray:
		return GL_TEXTURE_2D_ARRAY;
	case TextureType::Texture2DMultiSample:
		return GL_TEXTURE_2D_MULTISAMPLE;
	case TextureType::Texture2DMultiSampleArray:
		return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	case TextureType::Texture3D:
		return GL_TEXTURE_3D;
	case TextureType::CubeMap:
		return GL_TEXTURE_CUBE_MAP;
	case TextureType::CubeMapArray:
		return GL_TEXTURE_CUBE_MAP_ARRAY;
	case TextureType::Buffer:
		return GL_TEXTURE_BUFFER;
	default:
		break;
	}
	return GL_NONE;
}

void GLDevice::setRenderStateBlock(RenderStateBlock& renderStateBlock)
{
	// 开启深度写入与否
	if (renderStateBlock.m_depthState.m_writable.isDirty())
	{
		renderStateBlock.m_depthState.m_writable.clearDirty();
		glDepthMask(renderStateBlock.m_depthState.m_writable.getValue() ? GL_TRUE : GL_FALSE);
	}
	if (renderStateBlock.m_depthState.m_compareFunc.isDirty())
	{
		renderStateBlock.m_depthState.m_compareFunc.clearDirty();
		// 开启深度测试与否，及若开启配置哪种函数
		switch (renderStateBlock.m_depthState.m_compareFunc.getValue())
		{
		case CompareFunction::Disabled:
			glDisable(GL_DEPTH_TEST);
			break;
		case CompareFunction::Never:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_NEVER);
			break;
		case CompareFunction::Less:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			break;
		case CompareFunction::Equal:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_EQUAL);
			break;
		case CompareFunction::LessEqual:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			break;
		case CompareFunction::Greater:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_GREATER);
			break;
		case CompareFunction::NotEqual:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_NOTEQUAL);
			break;
		case CompareFunction::GreaterEqual:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_GEQUAL);
			break;
		case CompareFunction::Always:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_ALWAYS);
			break;
		default:
			break;
		}
	}
	if (renderStateBlock.m_colorState.m_rgbaWritable.isDirty())
	{
		renderStateBlock.m_colorState.m_rgbaWritable.clearDirty();
		glColorMask(renderStateBlock.m_colorState.m_rgbaWritable.getValue().x ? GL_TRUE : GL_FALSE,
			renderStateBlock.m_colorState.m_rgbaWritable.getValue().y ? GL_TRUE : GL_FALSE,
			renderStateBlock.m_colorState.m_rgbaWritable.getValue().z ? GL_TRUE : GL_FALSE,
			renderStateBlock.m_colorState.m_rgbaWritable.getValue().w ? GL_TRUE : GL_FALSE);
	}
	if (renderStateBlock.m_colorState.m_cullMode.isDirty())
	{
		renderStateBlock.m_colorState.m_cullMode.clearDirty();
		switch (renderStateBlock.m_colorState.m_cullMode.getValue())
		{
		case CullMode::Off:
			glDisable(GL_CULL_FACE);
			break;
		case CullMode::Front:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		case CullMode::Back:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		default:
			break;
		}
	}

	if (renderStateBlock.m_colorState.m_blendModeEnabled.isDirty())
	{
		renderStateBlock.m_colorState.m_blendModeEnabled.clearDirty();
		if (renderStateBlock.m_colorState.m_blendModeEnabled.getValue())
		{
			glEnable(GL_BLEND);
		}
		else {
			glDisable(GL_BLEND);
		}

	}
	if (renderStateBlock.m_colorState.m_srcBlendMode.isDirty())
	{
		renderStateBlock.m_colorState.m_srcBlendMode.clearDirty();
		auto srcBlendMode = getGLBlendMode(renderStateBlock.m_colorState.m_srcBlendMode.getValue());
		auto dstBlendMode = getGLBlendMode(renderStateBlock.m_colorState.m_dstBlendMode.getValue());
		glBlendFunc(srcBlendMode, dstBlendMode);
	}
}

void GLDevice::fillNoMaterialProperties(PipelineStateObject& pso, ShaderUniformProperty& uniformProperty)
{
	if (pso.m_globalTextureResources.find(uniformProperty.m_name) != pso.m_globalTextureResources.end())
	{
		auto texRes = pso.m_globalTextureResources[uniformProperty.m_name];
		if (texRes != nullptr)
		{
			bindTextureUnit(pso, uniformProperty.m_registerIndex, texRes);
		}
	}
}

void GLDevice::bindMesh(PipelineStateObject& pso)
{
	Shader::setGlobalMatrix(ShaderPropertyNames::ModelMatrix, pso.m_modelMatrix);

	// 保证顺序，先绑定VAO再绑定VBO，否则在绘制多个不同Mesh时会造成混乱
	glBindVertexArray(pso.m_meshIdentifier->getVAO());

	glBindBuffer(GL_ARRAY_BUFFER, pso.m_meshIdentifier->getVBO());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pso.m_meshIdentifier->getEBO());
}

void GLDevice::useProgram(PipelineStateObject& pso)
{
	glUseProgram(pso.m_program);
}

void GLDevice::bindTextureUnit(PipelineStateObject& pso, unsigned int registerIndex, TextureResourceIdentifier* textureIdentifier)
{
	if (textureIdentifier != nullptr)
	{
		pso.m_texUnit++;
		glBindTextureUnit(registerIndex, textureIdentifier->m_texture);
	}
}

void GLDevice::fillShaderProperties(PipelineStateObject& pso)
{
	auto& uniforms = pso.m_uniforms;
	auto material = pso.m_material;
	for (int i = 0; i < uniforms.size(); ++i)
	{
		auto matProperty = material == nullptr ? nullptr : material->getProperty(uniforms[i].m_name);
		if (matProperty != nullptr)
		{
			switch (matProperty->getMaterialPropertyType())
			{
			case MaterialPropertyType::Bool:
			{
				auto boolParam = static_cast<MaterialBoolProperty*>(matProperty.get());
				if (boolParam != nullptr)
				{
					glUniform1i(uniforms[i].m_location, boolParam->getValue() ? 1 : 0);
				}
				break;
			}
			case MaterialPropertyType::Int:
			{
				auto intParam = static_cast<MaterialIntProperty*>(matProperty.get());
				if (intParam != nullptr)
				{
					glUniform1i(uniforms[i].m_location, intParam->getValue());
				}
				break;
			}
			case MaterialPropertyType::Int2:
			{
				auto intParam = static_cast<MaterialInt2Property*>(matProperty.get());
				if (intParam != nullptr)
				{
					auto value = intParam->getValue();
					glUniform2i(uniforms[i].m_location, value.x, value.y);
				}
				break;
			}
			case MaterialPropertyType::Int3:
			{
				auto intParam = static_cast<MaterialInt3Property*>(matProperty.get());
				if (intParam != nullptr)
				{
					auto value = intParam->getValue();
					glUniform3i(uniforms[i].m_location, value.x, value.y, value.z);
				}
				break;
			}
			case MaterialPropertyType::Int4:
			{
				auto intParam = static_cast<MaterialInt4Property*>(matProperty.get());
				if (intParam != nullptr)
				{
					auto value = intParam->getValue();
					glUniform4i(uniforms[i].m_location, value.x, value.y, value.z, value.w);
				}
				break;
			}
			case MaterialPropertyType::Float:
			{
				auto floatParam = static_cast<MaterialFloatProperty*>(matProperty.get());
				if (floatParam != nullptr)
				{
					glUniform1f(uniforms[i].m_location, floatParam->getValue());
				}
				break;
			}
			case MaterialPropertyType::Vector2:
			{
				auto vec2Param = static_cast<MaterialVector2Property*>(matProperty.get());
				if (vec2Param != nullptr)
				{
					auto vec2 = vec2Param->getValue();
					glUniform2f(uniforms[i].m_location, vec2.x, vec2.y);
				}
				break;
			}
			case MaterialPropertyType::Vector3:
			{
				auto vec3Param = static_cast<MaterialVector3Property*>(matProperty.get());
				if (vec3Param != nullptr)
				{
					auto vec3 = vec3Param->getValue();
					glUniform3f(uniforms[i].m_location, vec3.x, vec3.y, vec3.z);
				}
				break;
			}
			case MaterialPropertyType::Vector4:
			{
				auto vec4Param = static_cast<MaterialVector4Property*>(matProperty.get());
				if (vec4Param != nullptr)
				{
					auto vec4 = vec4Param->getValue();
					glUniform4f(uniforms[i].m_location, vec4.x, vec4.y, vec4.z, vec4.w);
				}
				break;
			}
			case MaterialPropertyType::Vector4Array:
			{
				auto vec4ArrayParam = static_cast<MaterialVector4ArrayProperty*>(matProperty.get());
				if (vec4ArrayParam != nullptr)
				{
					auto vec4Array = vec4ArrayParam->getValue();
					auto size = vec4Array.size();
					if (size > 0)
					{
						glUniform4fv(uniforms[i].m_location, (GLsizei)size, &vec4Array[0].x);
					}
				}
				break;
			}
			case MaterialPropertyType::Matrix2:
			{
				auto mat2Param = static_cast<MaterialMatrix2Property*>(matProperty.get());
				if (mat2Param != nullptr)
				{
					auto mat2 = mat2Param->getValue();
					glUniformMatrix2fv(uniforms[i].m_location, 1, false, glm::value_ptr(mat2));
				}
				break;
			}
			case MaterialPropertyType::Matrix2x3:
			{
				auto mat2x3Param = static_cast<MaterialMatrix2x3Property*>(matProperty.get());
				if (mat2x3Param != nullptr)
				{
					auto mat2x3 = mat2x3Param->getValue();
					glUniformMatrix2x3fv(uniforms[i].m_location, 1, false, glm::value_ptr(mat2x3));
				}
				break;
			}
			case MaterialPropertyType::Matrix2x4:
			{
				auto mat2x4Param = static_cast<MaterialMatrix2x4Property*>(matProperty.get());
				if (mat2x4Param != nullptr)
				{
					auto mat2x4 = mat2x4Param->getValue();
					glUniformMatrix2x4fv(uniforms[i].m_location, 1, false, glm::value_ptr(mat2x4));
				}
				break;
			}
			case MaterialPropertyType::Matrix3x2:
			{
				auto mat3x2Param = static_cast<MaterialMatrix3x2Property*>(matProperty.get());
				if (mat3x2Param != nullptr)
				{
					auto mat3x2 = mat3x2Param->getValue();
					glUniformMatrix3x2fv(uniforms[i].m_location, 1, false, glm::value_ptr(mat3x2));
				}
				break;
			}
			case MaterialPropertyType::Matrix3:
			{
				auto mat3Param = static_cast<MaterialMatrix3Property*>(matProperty.get());
				if (mat3Param != nullptr)
				{
					auto mat3 = mat3Param->getValue();
					glUniformMatrix3fv(uniforms[i].m_location, 1, false, glm::value_ptr(mat3));
				}
				break;
			}
			case MaterialPropertyType::Matrix3x4:
			{
				auto mat3x4Param = static_cast<MaterialMatrix3x4Property*>(matProperty.get());
				if (mat3x4Param != nullptr)
				{
					auto mat3x4 = mat3x4Param->getValue();
					glUniformMatrix3x4fv(uniforms[i].m_location, 1, false, glm::value_ptr(mat3x4));
				}
				break;
			}
			case MaterialPropertyType::Matrix4x2:
			{
				auto mat4x2Param = static_cast<MaterialMatrix4x2Property*>(matProperty.get());
				if (mat4x2Param != nullptr)
				{
					auto mat4x2 = mat4x2Param->getValue();
					glUniformMatrix4x2fv(uniforms[i].m_location, 1, false, glm::value_ptr(mat4x2));
				}
				break;
			}
			case MaterialPropertyType::Matrix4x3:
			{
				auto mat4x3Param = static_cast<MaterialMatrix4x3Property*>(matProperty.get());
				if (mat4x3Param != nullptr)
				{
					auto mat4x3 = mat4x3Param->getValue();
					glUniformMatrix4x3fv(uniforms[i].m_location, 1, false, glm::value_ptr(mat4x3));
				}
				break;
			}
			case MaterialPropertyType::Matrix4:
			{
				auto mat4Param = static_cast<MaterialMatrix4Property*>(matProperty.get());
				if (mat4Param != nullptr)
				{
					auto mat4 = mat4Param->getValue();
					glUniformMatrix4fv(uniforms[i].m_location, 1, false, glm::value_ptr(mat4));
				}
				break;
			}
			case MaterialPropertyType::Texture:
			{
				auto textureParam = static_cast<MaterialTextureProperty*>(matProperty.get());
				if (textureParam != nullptr && uniforms[i].isSampler())
				{
					auto texture = textureParam->getValue();
					for (const auto& texIdentifierPtr : pso.m_textureResources)
					{
						if (texIdentifierPtr->getInstanceId() == texture->getInstanceId())
						{
							bindTextureUnit(pso, uniforms[i].m_registerIndex, texIdentifierPtr);
							break;
						}
					}
				}
				break;
			}
			default:
				break;
			}
		}
		else {
			fillNoMaterialProperties(pso, uniforms[i]);
		}
	}
}

void GLDevice::fillMaterialPropertyBlocks(PipelineStateObject& pso)
{
	// 全局缓冲，逐材质设置
	auto& constantBufferSet = Shader::getShaderConstantBufferSet();
	auto& blockRefs = pso.m_uniformBlockRefs;
	auto material = pso.m_material;
	if (material == nullptr)
	{
		return;
	}
	for (int i = 0; i < blockRefs.size(); ++i)
	{
		auto& blockRef = blockRefs[i];
		auto blockPtr = constantBufferSet.findBlock(blockRef.m_uniformBlockName);
		auto uniformsInBlock = blockPtr->m_blockUniforms;
		for (int j = 0; j < uniformsInBlock.size(); ++j)
		{
			auto& uniform = uniformsInBlock[j];
			auto matProperty = material->getProperty(uniform.m_uniformName);

			switch (matProperty->getMaterialPropertyType())
			{
			case MaterialPropertyType::Bool:
			{
				auto boolParam = static_cast<MaterialBoolProperty*>(matProperty.get());
				if (boolParam != nullptr)
				{
					Shader::setGlobalBool(uniform.m_uniformName.c_str(), boolParam->getValue());
				}
				break;
			}
			case MaterialPropertyType::Int:
			{
				auto intParam = static_cast<MaterialIntProperty*>(matProperty.get());
				if (intParam != nullptr)
				{
					Shader::setGlobalInt(uniform.m_uniformName.c_str(), intParam->getValue());
				}
				break;
			}
			case MaterialPropertyType::Float:
			{
				auto floatParam = static_cast<MaterialFloatProperty*>(matProperty.get());
				if (floatParam != nullptr)
				{
					Shader::setGlobalFloat(uniform.m_uniformName.c_str(), floatParam->getValue());
				}
				break;
			}
			case MaterialPropertyType::Vector4:
			{
				auto vec4Param = static_cast<MaterialVector4Property*>(matProperty.get());
				if (vec4Param != nullptr)
				{
					Shader::setGlobalVector(uniform.m_uniformName.c_str(), vec4Param->getValue());
				}
				break;
			}
			case MaterialPropertyType::Matrix4:
			{
				auto mat4Param = static_cast<MaterialMatrix4Property*>(matProperty.get());
				if (mat4Param != nullptr)
				{
					Shader::setGlobalMatrix(uniform.m_uniformName.c_str(), mat4Param->getValue());
				}
				break;
			}
			default:
				break;
			}
		}

	}
	uploadConstantBufferResource(ConstantBufferType::PerPass);
	bindBlockForProgram(pso);
}

void GLDevice::drawElements(PipelineStateObject& pso)
{
	// 绘制指令
	glDrawElements(GL_TRIANGLES, pso.m_meshIdentifier->getIndicesCount(), GL_UNSIGNED_SHORT, NULL);
}

void GLDevice::draw(PipelineStateObject& pso)
{
	GLuint program = pso.m_program;
	if (program <= 0 || !glIsProgram(program))
	{
		return;
	}

	bindMesh(pso);

	useProgram(pso);

	fillShaderProperties(pso);

	fillMaterialPropertyBlocks(pso);

	drawElements(pso);
}

void GLDevice::uploadConstantBufferResource(ConstantBufferType constantBufferType)
{
	auto constantBufferSet = Shader::getShaderConstantBufferSet();

	auto identifier = constantBufferSet.getGlobalBufferIdentifier(constantBufferType);
	auto buffer = constantBufferSet.getGlobalByteBuffer(constantBufferType);

	if (identifier != nullptr)
	{
		GLuint ubo = identifier->getUbo();
		glNamedBufferSubData(ubo, 0, buffer->dataSize(), buffer->data());
	}
}