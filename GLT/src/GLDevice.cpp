#include "GLDevice.h"

std::vector<MeshResourceIdentifier> GLDevice::requestMeshResources(std::vector<Mesh*>& meshPtrs)
{
	GLsizei length = (GLsizei)meshPtrs.size();
	std::vector<MeshResourceIdentifier> meshResourceIdentifiers = std::vector<MeshResourceIdentifier>(length);
	std::vector<GLuint> newMeshVaos = std::vector<GLuint>(length);
	std::vector<GLuint> newMeshVbos = std::vector<GLuint>(length);
	std::vector<GLuint> newMeshEbos = std::vector<GLuint>(length);
	// ����VAO��VBO��EBO����
	glCreateVertexArrays(length, newMeshVaos.data());
	glCreateBuffers(length, newMeshVbos.data());
	glCreateBuffers(length, newMeshEbos.data());
	for (int i = 0; i < length; ++i)
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

		if (verticesMemorySize > 0)
		{
			// 0��λ�����ԣ�4����������ɵ�������ָ��ָ�򻺴���ʼλ��
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);
			/*glBufferData(GL_ARRAY_BUFFER, verticesMemorySize + colorsMemorySize, NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, verticesMemorySize, mesh->getVertices());*/
			glNamedBufferStorage(vbo, verticesMemorySize + colorsMemorySize + uvsMemorySize + normalsMemorySize, NULL, GL_DYNAMIC_STORAGE_BIT);
			glNamedBufferSubData(vbo, 0, verticesMemorySize, mesh->getVertices());
			if (colorsMemorySize > 0)
			{
				auto offset = verticesMemorySize;
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				glEnableVertexAttribArray(1);
				glNamedBufferSubData(vbo, offset, colorsMemorySize, mesh->getColors());
				//glBufferSubData(GL_ARRAY_BUFFER, verticesMemorySize, colorsMemorySize, mesh->getColors());
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
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

	// ������࣬�������������������
	for (int i = 0; i < newTextureCount; ++i)
	{
		auto textureType = texturePtrs[i]->getTextureType();;
		if (textureMap.find(textureType) == textureMap.end())
		{
			textureMap[textureType] = std::vector<int>();
		}
		textureMap[textureType].push_back(i);
	}

	// ��������������󼰱�ʶ��
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
		// ����N��������󣬲����ɱ�ʶ��
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
				identifier.m_perChannelSize = texturePtr->getPerChannelSize();
				identifier.m_externalFormat = texturePtr->getExternalFormat();

				identifier.m_texture = tempTextures[i];
				textureResourceIdentifiers[index] = identifier;
			}
		}
	}

	// ����洢�ռ�
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
			glBindTextureUnit(0, resourceIdentifier.m_texture);

			setTextureWrapMode(resourceIdentifier.m_texture, GL_TEXTURE_WRAP_S, texture2DPtr->getWrapModeS());
			setTextureWrapMode(resourceIdentifier.m_texture, GL_TEXTURE_WRAP_T, texture2DPtr->getWrapModeT());

			setTextureFilter(resourceIdentifier.m_texture, GL_TEXTURE_MIN_FILTER, texture2DPtr->getTextureFilter());
			setTextureFilter(resourceIdentifier.m_texture, GL_TEXTURE_MAG_FILTER, texture2DPtr->getTextureFilter());

			glTextureStorage2D(resourceIdentifier.m_texture, resourceIdentifier.m_levels, internalFormat, resourceIdentifier.m_width, resourceIdentifier.m_height);
			glTextureSubImage2D(resourceIdentifier.m_texture, 0, 0, 0, resourceIdentifier.m_width,
				resourceIdentifier.m_height, externalFormat, perChannelSize, texturePtr->getData());

			glGenerateTextureMipmap(resourceIdentifier.m_texture);
		}
		else if (textureType == TextureType::Texture3D)
		{
			glTextureStorage3D(resourceIdentifier.m_texture, resourceIdentifier.m_levels, internalFormat, resourceIdentifier.m_width, resourceIdentifier.m_height,
				resourceIdentifier.m_depth);
		}
		// Todo��������
	}

	return textureResourceIdentifiers;
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
		// Sampler -> SamplerResourceIdentifier ��������

		samplerResourceIdentifiers[i] = samplerResourceIdentifier;
	}

	// �����������
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

	// ����RT����
	std::vector<GLuint> rts = std::vector<GLuint>(newRenderTextureCount);
	glCreateFramebuffers(newRenderTextureCount, rts.data());
	for (size_t i = 0; i < newRenderTextureCount; ++i)
	{
		auto rtPtr = renderTargetPtrs[i];
		renderTargetResources[i] = RenderTargetIdentifier(rtPtr->getInstanceId());
		auto& rtIdentifier = renderTargetResources[i];
		auto fbo = rts[i];
		rtIdentifier.m_fbo = fbo;
		auto attachmentIdentifiers = rtsAttachmentIdentifiers[i];
		auto descriptor = rtPtr->getRenderTargetDescriptor();

		rtIdentifier.m_attachmentIdentifiers = attachmentIdentifiers;

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
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	// ����RenderBuffer
	glCreateRenderbuffers((GLsizei)renderBufferPtrs.size(), renderBufferIds.data());
	for (int i = 0; i < renderBufferPtrs.size(); ++i)
	{
		RenderBufferIdentifier identifier(renderBufferPtrs[i]->getInstanceId());
		identifier.m_renderBuffer = renderBufferIds[i];
		identifier.m_isDepthBuffer = renderBufferPtrs[i]->IsDepthBuffer();
		if (identifier.m_isDepthBuffer)
		{
			if (renderBufferPtrs[i]->getDepthStencilType() != RenderTextureDepthStencilType::None)
			{
				identifier.m_depthStencilType = renderBufferPtrs[i]->getDepthStencilType();
				identifier.m_width = renderBufferPtrs[i]->getWidth();
				identifier.m_height = renderBufferPtrs[i]->getHeight();
				// ����RenderBuffer�洢�ռ�
				glNamedRenderbufferStorage(renderBufferIds[i], getGLDepthStencilType(identifier.m_depthStencilType), identifier.m_width, identifier.m_height);
			}
		}
		else {
			if (renderBufferPtrs[i]->getColorInternalFormat() != TextureInternalFormat::None)
			{
				identifier.m_internalFormat = renderBufferPtrs[i]->getColorInternalFormat();
				identifier.m_width = renderBufferPtrs[i]->getWidth();
				identifier.m_height = renderBufferPtrs[i]->getHeight();
				// ����RenderBuffer�洢�ռ�
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
		// ����洢�ռ�
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

void GLDevice::activate(RenderTargetIdentifier* rtIdentifier)
{
	if (rtIdentifier == nullptr)
	{
		return;
	}
	if (rtIdentifier->m_fbo > 0)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rtIdentifier->m_fbo);
		m_curRT = rtIdentifier;
	}
}

void GLDevice::clearColor(float r, float g, float b, float a)
{
	//auto size = Window::getInstance()->getSize();
	//glViewport(0, 0, size.x, size.y);
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLDevice::blitToWindow()
{
	if (m_curRT == nullptr)
	{
		return;
	}
	auto windowSize = Window::getInstance()->getSize();
	assert(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	// ��FBO�󶨵���ȡFBĿ����
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_curRT->m_fbo);
	// ���ƴ�������Ϊ0����˼�����°󶨵����ڵ�֡������
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, windowSize.x, windowSize.y, 0, 0, windowSize.x, windowSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void GLDevice::bindBlockForProgram(Shader& shader)
{
	ConstantBufferSet& globalBuffer = Shader::getShaderConstantBufferSet();
	std::vector<ShaderUniformBlockReference>& blockRefs = shader.getReferencedBlocks();
	auto program = shader.getShaderProgram();
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
	// �������д�����
	glDepthRange(0, 1);
	glDepthMask(renderStateBlock.m_depthState.m_writable ? GL_TRUE : GL_FALSE);
	glColorMask(renderStateBlock.m_colorState.m_rgbaWritable.x ? GL_TRUE : GL_FALSE,
		renderStateBlock.m_colorState.m_rgbaWritable.y ? GL_TRUE : GL_FALSE,
		renderStateBlock.m_colorState.m_rgbaWritable.z ? GL_TRUE : GL_FALSE,
		renderStateBlock.m_colorState.m_rgbaWritable.w ? GL_TRUE : GL_FALSE);
	// ������Ȳ�����񣬼��������������ֺ���
	switch (renderStateBlock.m_depthState.m_compareFunc)
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
	switch (renderStateBlock.m_colorState.m_cullMode)
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

void GLDevice::drawMesh(Mesh* mesh, Material* material, glm::mat4 modelMatrix, MeshResourceIdentifier* meshResourceIdentifier, std::vector<TextureResourceIdentifier*>& textureResources)
{
	Shader::setGlobalMatrix(ShaderPropertyNames::ModelMatrix, modelMatrix);

	// ��֤˳���Ȱ�VAO�ٰ�VBO�������ڻ��ƶ����ͬMeshʱ����ɻ���
	glBindVertexArray(meshResourceIdentifier->getVAO());

	glBindBuffer(GL_ARRAY_BUFFER, meshResourceIdentifier->getVBO());

	auto shader = material->getShader();
	GLuint program = shader->getShaderProgram();

	if (program > 0 && glIsProgram(program))
	{
		glUseProgram(program);

		// uniform��������
		auto& uniforms = shader->getShaderUniforms();
		int texCount = 0;
		for (int i = 0; i < uniforms.size(); ++i)
		{
			auto matProperty = material->getProperty(uniforms[i].m_name);
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
				case MaterialPropertyType::Float:
				{
					auto floatParam = static_cast<MaterialFloatProperty*>(matProperty.get());
					if (floatParam != nullptr)
					{
						glUniform1f(uniforms[i].m_location, floatParam->getValue());
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
					if (textureParam != nullptr)
					{
						auto texture = textureParam->getTexture();
						if (texture != nullptr)
						{
							for (const auto& texIdentifierPtr : textureResources)
							{
								if (texIdentifierPtr->getInstanceId() == texture->getInstanceId())
								{
									auto target = textureType2TextureTarget(texIdentifierPtr->m_textureType);
									if (target != GL_NONE)
									{
										glBindTextureUnit(texCount++, texIdentifierPtr->m_texture);
									}
									break;
								}
							}
						}
					}
					break;
				}
				default:
					break;
				}
			}
		}
		// ȫ�ֻ��壬���������
		auto& constantBufferSet = Shader::getShaderConstantBufferSet();
		auto& blockRefs = shader->getReferencedBlocks();
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
		bindBlockForProgram(*shader.get());
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshResourceIdentifier->getEBO());
	// ����ָ��
	glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_SHORT, NULL);
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