#include "GLDevice.h"
std::vector<MeshResourceIdentifier> GLDevice::requestMeshResources(std::vector<Mesh*>& meshPtrs)
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
	for (int i = 0;i < length;++i)
	{
		GLuint vao = newMeshVaos[i];
		GLuint vbo = newMeshVbos[i];
		GLuint ebo = newMeshEbos[i];
		auto mesh = meshPtrs[i];
		GLuint instanceId = mesh->getInstanceId();
		size_t verticesMemorySize = mesh->getVerticesCount() * Mesh::VertexSize;
		size_t colorsMemorySize = mesh->getVerticesCount() * Mesh::ColorSize;
		size_t uvsMemorySize = mesh->getVerticesCount() * Mesh::UVSize;
		size_t normalsMemorySize = mesh->getVerticesCount() * Mesh::NormalSize;

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
			glNamedBufferSubData(vbo, 0, verticesMemorySize, mesh->getVertices());
			if (colorsMemorySize > 0)
			{
				auto offset = verticesMemorySize;
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				glEnableVertexAttribArray(1);
				glNamedBufferSubData(vbo, offset, colorsMemorySize, mesh->getColors());
				//glBufferSubData(GL_ARRAY_BUFFER, verticesMemorySize, colorsMemorySize, mesh->GetColors());
			}
			if (uvsMemorySize > 0)
			{
				auto offset = colorsMemorySize + verticesMemorySize;
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				glEnableVertexAttribArray(2);
				glNamedBufferSubData(vbo, offset, uvsMemorySize, mesh->getUvs());
			}
			if (normalsMemorySize > 0)
			{
				auto offset = colorsMemorySize + verticesMemorySize + uvsMemorySize;
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				glEnableVertexAttribArray(3);
				glNamedBufferSubData(vbo, offset, normalsMemorySize, mesh->getNormals());
			}
		}
		glNamedBufferStorage(ebo, mesh->getIndicesCount() * Mesh::IndexSize, mesh->getIndices(), GL_DYNAMIC_STORAGE_BIT);

		meshResourceIdentifiers[i] = MeshResourceIdentifier(vao, vbo, ebo, instanceId);
	}
	return meshResourceIdentifiers;
}

void GLDevice::destroyMeshResources(std::vector<MeshResourceIdentifier>& meshIdentifiers)
{
	GLsizei removedCount = (GLsizei)meshIdentifiers.size();
	std::vector<GLuint> removedVaos = std::vector<GLuint>(removedCount);
	std::vector<GLuint> removedVbos = std::vector<GLuint>(removedCount);
	std::vector<GLuint> removedEbos = std::vector<GLuint>(removedCount);
	for (size_t i = 0;i < removedCount;++i)
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
	for (int i = 0;i < newTextureCount;++i)
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
		GLenum target = Texture::textureType2TextureTarget(textureType);
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
			for (int i = 0;i < size;++i)
			{
				int index = textureIndices[i];
				auto texturePtr = texturePtrs[index];
				auto identifier = TextureResourceIdentifier();
				identifier.m_levels = texturePtr->GetLevels();
				identifier.m_internalFormat = texturePtr->GetInternalFormat();
				identifier.m_textureType = textureType;
				identifier.m_width = texturePtr->GetWidth();
				if (textureType == TextureType::Texture2D)
				{
					auto texture2DPtr = static_cast<Texture2D*>(texturePtr);
					identifier.m_height = texture2DPtr->GetHeight();
				}
				else if (textureType == TextureType::Texture3D)
				{
					auto texture3DPtr = static_cast<Texture3D*>(texturePtr);
					identifier.m_height = texture3DPtr->GetHeight();
					identifier.m_depth = texture3DPtr->GetDepth();
				}
				identifier.m_perChannelSize = texturePtr->GetPerChannelSize();
				identifier.m_externalFormat = texturePtr->GetExternalFormat();

				identifier.m_texture = tempTextures[i];
				textureResourceIdentifiers[index] = identifier;
			}
		}
	}

	// 分配存储空间
	for (int i = 0;i < newTextureCount;++i)
	{
		const auto& texturePtr = texturePtrs[i];
		auto& resourceIdentifier = textureResourceIdentifiers[i];
		auto textureType = resourceIdentifier.m_textureType;
		if (textureType == TextureType::Texture1D)
		{
			glTextureStorage1D(resourceIdentifier.m_texture, resourceIdentifier.m_levels, resourceIdentifier.m_internalFormat, resourceIdentifier.m_width);
		}
		else if (textureType == TextureType::Texture2D)
		{
			auto texture2DPtr = static_cast<Texture2D*>(texturePtr);
			glBindTextureUnit(0, resourceIdentifier.m_texture);

			setTextureWrapMode(resourceIdentifier.m_texture, GL_TEXTURE_WRAP_S, texture2DPtr->GetWrapModeS());
			setTextureWrapMode(resourceIdentifier.m_texture, GL_TEXTURE_WRAP_T, texture2DPtr->GetWrapModeT());

			setTextureFilter(resourceIdentifier.m_texture, GL_TEXTURE_MIN_FILTER, texture2DPtr->GetTextureFilter());
			setTextureFilter(resourceIdentifier.m_texture, GL_TEXTURE_MAG_FILTER, texture2DPtr->GetTextureFilter());

			glTextureStorage2D(resourceIdentifier.m_texture, resourceIdentifier.m_levels, resourceIdentifier.m_internalFormat, resourceIdentifier.m_width, resourceIdentifier.m_height);
			glTextureSubImage2D(resourceIdentifier.m_texture, 0, 0, 0, resourceIdentifier.m_width,
				resourceIdentifier.m_height, resourceIdentifier.m_externalFormat, resourceIdentifier.m_perChannelSize, texturePtr->getData());

			glGenerateTextureMipmap(resourceIdentifier.m_texture);
		}
		else if (textureType == TextureType::Texture3D)
		{
			glTextureStorage3D(resourceIdentifier.m_texture, resourceIdentifier.m_levels, resourceIdentifier.m_internalFormat, resourceIdentifier.m_width, resourceIdentifier.m_height,
				resourceIdentifier.m_depth);
		}
		// Todo其他类型
	}

	return textureResourceIdentifiers;
}

void GLDevice::destroyTextureResources(std::vector<TextureResourceIdentifier>& textureIdentifiers)
{
	size_t length = textureIdentifiers.size();
	std::vector<GLuint> removedTextures = std::vector<GLuint>(length);
	for (size_t i = 0;i < length;++i)
	{
		removedTextures[i] = textureIdentifiers[i].m_texture;
	}
	glDeleteTextures((GLsizei)removedTextures.size(), removedTextures.data());
}

std::vector<SamplerResouceIdentifier> GLDevice::requestSamplerResources(std::vector<Sampler*>& samplerPtrs)
{
	GLsizei newSamplerCount = (GLsizei)samplerPtrs.size();
	std::vector<SamplerResouceIdentifier> samplerResourceIdentifiers = std::vector<SamplerResouceIdentifier>(newSamplerCount);
	for (int i = 0;i < newSamplerCount;++i)
	{
		SamplerResouceIdentifier samplerResourceIdentifier;
		// Sampler -> SamplerResourceIdentifier 参数设置

		samplerResourceIdentifiers[i] = samplerResourceIdentifier;
	}

	// 创建纹理对象
	std::vector<GLuint> samplers = std::vector<GLuint>(newSamplerCount);
	glCreateSamplers(newSamplerCount, samplers.data());
	for (size_t i = 0;i < newSamplerCount;++i)
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
	for (size_t i = 0;i < length;++i)
	{
		removedSamplers[i] = samplerIdentifiers[i].m_sampler;
	}
	glDeleteSamplers((GLsizei)removedSamplers.size(), removedSamplers.data());
}

void GLDevice::requestConstantBufferResource(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers)
{
	int size = (int)constantBufferIdentifiers.size();
	std::vector<GLuint> ubos = std::vector<GLuint>(size);
	glCreateBuffers(size, ubos.data());
	for (int i = 0;i < size;++i)
	{
		constantBufferIdentifiers[i].SetUbo(ubos[i]);
		size_t bufferSize = constantBufferIdentifiers[i].getTotalBufferSize();
		// 分配存储空间
		glBindBuffer(GL_UNIFORM_BUFFER, ubos[i]);
		glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
		//glNamedBufferStorage(ubo, bufferSize, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void GLDevice::destroyConstantBufferResource(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers)
{
	size_t length = constantBufferIdentifiers.size();
	std::vector<GLuint> removedBuffers = std::vector<GLuint>(length);
	for (size_t i = 0;i < length;++i)
	{
		removedBuffers[i] = constantBufferIdentifiers[i].GetUbo();
	}
	glDeleteBuffers((GLsizei)removedBuffers.size(), removedBuffers.data());
}

std::vector<RenderTargetIdentifier> GLDevice::requestRenderTargetResource(std::vector<RenderTarget*>& renderTargetPtrs)
{
	GLsizei newRenderTextureCount = (GLsizei)renderTargetPtrs.size();
	std::vector<RenderTargetIdentifier> renderTargetResources = std::vector<RenderTargetIdentifier>(newRenderTextureCount);

	for (int i = 0;i < newRenderTextureCount;++i)
	{
		RenderTargetIdentifier rtResourceIdentifier;
		renderTargetResources[i] = rtResourceIdentifier;
	}

	// 创建RT对象
	std::vector<GLuint> rts = std::vector<GLuint>(newRenderTextureCount);
	glCreateFramebuffers(newRenderTextureCount, rts.data());
	for (size_t i = 0;i < newRenderTextureCount;++i)
	{
		auto rtIdentifier = renderTargetResources[i];
		rtIdentifier.m_fbo = rts[i];
	}
	for (size_t i = 0;i < newRenderTextureCount;++i)
	{
		auto rtPtr = renderTargetPtrs[i];
		auto& rtIdentifier = renderTargetResources[i];
		auto fbo = rtIdentifier.m_fbo;

		auto descriptor = rtPtr->GetRenderTargetDescriptor();
		auto& colorRenderBuffer = rtIdentifier.m_colorRenderBuffer;
		auto& depthRenderBuffer = rtIdentifier.m_depthRenderBuffer;
		auto& stencilRenderBuffer = rtIdentifier.m_stencilRenderBuffer;
		if (descriptor.m_colorInternalFormat != GL_NONE)
		{
			colorRenderBuffer.create(descriptor.m_width, descriptor.m_height, descriptor.m_colorInternalFormat);
			// 将RenderBuffer关联到FrameBuffer的附件
			glNamedFramebufferRenderbuffer(fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer.getRenderBufferPtr());
		}
		if (descriptor.m_depthInternalFormat == RenderTextureDepthStencilType::Depth_Stencil)
		{
			depthRenderBuffer.create(descriptor.m_width, descriptor.m_height, GL_DEPTH_STENCIL);
			// 将RenderBuffer关联到FrameBuffer的附件
			glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer.getRenderBufferPtr());
		}
		else
		{
			if (descriptor.m_depthInternalFormat != RenderTextureDepthStencilType::None)
			{
				depthRenderBuffer.create(descriptor.m_width, descriptor.m_height, getDepthStencilGLType(descriptor.m_depthInternalFormat));
				// 将RenderBuffer关联到FrameBuffer的附件
				glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer.getRenderBufferPtr());
			}
			if (descriptor.m_stencilInternalFormat != RenderTextureDepthStencilType::None)
			{
				stencilRenderBuffer.create(descriptor.m_width, descriptor.m_height, getDepthStencilGLType(descriptor.m_stencilInternalFormat));
				// 将RenderBuffer关联到FrameBuffer的附件
				glNamedFramebufferRenderbuffer(fbo, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilRenderBuffer.getRenderBufferPtr());
			}
		}
	}
	return renderTargetResources;
}

void GLDevice::destroyRenderTargetResource(std::vector<RenderTargetIdentifier>& renderTargetIdentifiers)
{
	size_t length = renderTargetIdentifiers.size();
	std::vector<GLuint> removedRTs;
	for (size_t i = 0;i < length;++i)
	{
		removedRTs.push_back(renderTargetIdentifiers[i].m_fbo);

		auto& colorRenderBuffer = renderTargetIdentifiers[i].m_colorRenderBuffer;
		auto& depthRenderBuffer = renderTargetIdentifiers[i].m_depthRenderBuffer;
		auto& stencilRenderBuffer = renderTargetIdentifiers[i].m_stencilRenderBuffer;
		colorRenderBuffer.release();
		depthRenderBuffer.release();
		stencilRenderBuffer.release();
	}
	glDeleteFramebuffers((GLsizei)removedRTs.size(), removedRTs.data());
}

void GLDevice::activate(RenderTargetIdentifier& rtIdentifier)
{
	if (rtIdentifier.m_fbo > 0)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rtIdentifier.m_fbo);
		m_curRT = rtIdentifier;
	}
}

void GLDevice::clearColor(float r, float g, float b, float a)
{
	auto size = Window::getInstance()->getSize();
	glViewport(0, 0, size.x, size.y);
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLDevice::blitToWindow()
{
	auto windowSize = Window::getInstance()->getSize();
	assert(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	// 将FBO绑定到读取FB目标上
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_curRT.m_fbo);
	// 绘制窗口设置为0，意思是重新绑定到窗口的帧缓存上
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, windowSize.x, windowSize.y, 0, 0, windowSize.x, windowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}