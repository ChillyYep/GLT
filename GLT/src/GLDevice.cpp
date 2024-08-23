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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		if (verticesMemorySize > 0)
		{
			// 0��λ�����ԣ�4����������ɵ�������ָ��ָ�򻺴���ʼλ��
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

			setTextureWrapMode(resourceIdentifier.m_texture, GL_TEXTURE_WRAP_S, texture2DPtr->GetWrapModeS());
			setTextureWrapMode(resourceIdentifier.m_texture, GL_TEXTURE_WRAP_T, texture2DPtr->GetWrapModeT());

			setTextureFilter(resourceIdentifier.m_texture, GL_TEXTURE_MIN_FILTER, texture2DPtr->GetTextureFilter());
			setTextureFilter(resourceIdentifier.m_texture, GL_TEXTURE_MAG_FILTER, texture2DPtr->GetTextureFilter());

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
		SamplerResouceIdentifier samplerResourceIdentifier;
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

std::vector<RenderTargetIdentifier> GLDevice::requestRenderTargetResource(std::vector<RenderTarget*>& renderTargetPtrs)
{
	GLsizei newRenderTextureCount = (GLsizei)renderTargetPtrs.size();
	std::vector<RenderTargetIdentifier> renderTargetResources = std::vector<RenderTargetIdentifier>(newRenderTextureCount);

	for (int i = 0; i < newRenderTextureCount; ++i)
	{
		RenderTargetIdentifier rtResourceIdentifier;
		renderTargetResources[i] = rtResourceIdentifier;
	}

	// ����RT����
	std::vector<GLuint> rts = std::vector<GLuint>(newRenderTextureCount);
	glCreateFramebuffers(newRenderTextureCount, rts.data());
	for (size_t i = 0; i < newRenderTextureCount; ++i)
	{
		auto rtIdentifier = renderTargetResources[i];
		rtIdentifier.m_fbo = rts[i];
	}
	std::vector<RenderBuffer*> renderBuffers;
	std::vector<Texture*> textures;
	for (size_t i = 0; i < newRenderTextureCount; ++i)
	{
		auto rtPtr = renderTargetPtrs[i];
		auto& rtIdentifier = renderTargetResources[i];
		auto fbo = rtIdentifier.m_fbo;
		auto attachments = rtPtr->GetAttachments();
		for (const auto& attachment : attachments)
		{
			auto resourceType = attachment.GetResourceType();
			if (resourceType == FBOAttachmentResourceType::RenderBuffer)
			{
				renderBuffers.push_back(attachment.GetRenderBuffer());
			}
			else if (resourceType == FBOAttachmentResourceType::Texture)
			{
				textures.push_back(attachment.GetTexture());
			}
		}
		auto descriptor = rtPtr->GetRenderTargetDescriptor();
		requestRenderBufferResources(std::vector<RenderBuffer*>() { &rtIdentifier.m_colorRenderBuffer, & rtIdentifier.m_colorRenderBuffer, & rtIdentifier.m_colorRenderBuffer });
		auto& colorRenderBuffer = rtIdentifier.m_colorRenderBuffer;
		auto& depthRenderBuffer = rtIdentifier.m_depthRenderBuffer;
		auto& stencilRenderBuffer = rtIdentifier.m_stencilRenderBuffer;
		if (descriptor.m_colorInternalFormat != TextureInternalFormat::None)
		{
			auto colorInternalFormat = getGLTextureInternalFormat(descriptor.m_colorInternalFormat);
			colorRenderBuffer.create(descriptor.m_width, descriptor.m_height, colorInternalFormat);
			// ��RenderBuffer������FrameBuffer�ĸ���
			glNamedFramebufferRenderbuffer(fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer.getRenderBufferPtr());
		}
		if (descriptor.m_depthInternalFormat == RenderTextureDepthStencilType::Depth_Stencil)
		{
			depthRenderBuffer.create(descriptor.m_width, descriptor.m_height, getDepthStencilGLType(descriptor.m_depthInternalFormat));
			// ��RenderBuffer������FrameBuffer�ĸ���
			glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer.getRenderBufferPtr());
		}
		else
		{
			if (descriptor.m_depthInternalFormat != RenderTextureDepthStencilType::None)
			{
				depthRenderBuffer.create(descriptor.m_width, descriptor.m_height, getDepthStencilGLType(descriptor.m_depthInternalFormat));
				// ��RenderBuffer������FrameBuffer�ĸ���
				glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer.getRenderBufferPtr());
			}
			if (descriptor.m_stencilInternalFormat != RenderTextureDepthStencilType::None)
			{
				stencilRenderBuffer.create(descriptor.m_width, descriptor.m_height, getDepthStencilGLType(descriptor.m_stencilInternalFormat));
				// ��RenderBuffer������FrameBuffer�ĸ���
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
	for (size_t i = 0; i < length; ++i)
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

std::vector<RenderBufferIdentifier> GLDevice::requestRenderBufferResources(std::vector<RenderBuffer*>& renderBufferPtrs)
{
	std::vector<GLuint> renderBufferIds = std::vector<GLuint>(renderBufferPtrs.size());
	std::vector<RenderBufferIdentifier> identifiers = std::vector<RenderBufferIdentifier>(renderBufferPtrs.size());
	// ����RenderBuffer
	glCreateRenderbuffers(renderBufferPtrs.size(), renderBufferIds.data());
	for (int i = 0; i < renderBufferPtrs.size(); ++i)
	{
		RenderBufferIdentifier identifier;
		if (renderBufferPtrs[i]->GetInternalFormat() != GL_NONE)
		{
			identifier.m_internalFormat = renderBufferPtrs[i]->GetInternalFormat();
			identifier.m_width = renderBufferPtrs[i]->GetWidth();
			identifier.m_height = renderBufferPtrs[i]->GetHeight();
			// ����RenderBuffer�洢�ռ�
			glNamedRenderbufferStorage(renderBufferIds[i], identifier.m_internalFormat, identifier.m_width, identifier.m_height);
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
	glDeleteRenderbuffers(renderBufferIds.size(), renderBufferIds.data());
}

void GLDevice::requestConstantBufferResources(std::vector<ConstantBufferIdentifier>& constantBufferIdentifiers)
{
	int size = (int)constantBufferIdentifiers.size();
	std::vector<GLuint> ubos = std::vector<GLuint>(size);
	glCreateBuffers(size, ubos.data());
	for (int i = 0; i < size; ++i)
	{
		constantBufferIdentifiers[i].SetUbo(ubos[i]);
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
		ubos[i] = constantBufferIdentifiers[i].GetUbo();
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
	auto size = Window::getInstance()->getSize();
	glViewport(0, 0, size.x, size.y);
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
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, globalBufferIdentifier.GetUbo());
	for (int i = 0; i < blockRefs.size(); ++i)
	{
		auto blockIndex = blockRefs[i].m_blockIndex;
		ShaderUniformBlockProperty* block;

		ConstantBufferIdentifier* globalBufferIdentifier = globalBuffer.getGlobalBufferIdentifierByBlockName(blockRefs[i].m_uniformBlockName);
		if (globalBufferIdentifier->findBlock(blockRefs[i].m_uniformBlockName, block))
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, block->m_blockBindingNum, globalBufferIdentifier->GetUbo(), block->m_blockOffset, block->m_preDefineSize);
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
void GLDevice::drawMesh(Mesh* mesh, Material* material, glm::mat4 modelMatrix, MeshResourceIdentifier* meshResourceIdentifier, std::unordered_map<GLuint, TextureResourceIdentifier>& textureResources)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDepthRange(0, 1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Shader::setGlobalMatrix(ShaderPropertyNames::ModelMatrix, modelMatrix);
	// ��Mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshResourceIdentifier->getEBO());

	glBindVertexArray(meshResourceIdentifier->getVAO());

	glBindBuffer(GL_ARRAY_BUFFER, meshResourceIdentifier->getVBO());

	auto shader = material->GetShader();
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
						glUniform1i(uniforms[i].m_location, boolParam->GetValue() ? 1 : 0);
					}
					break;
				}
				case MaterialPropertyType::Int:
				{
					auto intParam = static_cast<MaterialIntProperty*>(matProperty.get());
					if (intParam != nullptr)
					{
						glUniform1i(uniforms[i].m_location, intParam->GetValue());
					}
					break;
				}
				case MaterialPropertyType::Float:
				{
					auto floatParam = static_cast<MaterialFloatProperty*>(matProperty.get());
					if (floatParam != nullptr)
					{
						glUniform1f(uniforms[i].m_location, floatParam->GetValue());
					}
					break;
				}
				case MaterialPropertyType::Vector4:
				{
					auto vec4Param = static_cast<MaterialVector4Property*>(matProperty.get());
					if (vec4Param != nullptr)
					{
						auto vec4 = vec4Param->GetValue();
						glUniform4f(uniforms[i].m_location, vec4.x, vec4.y, vec4.z, vec4.w);
					}
					break;
				}
				case MaterialPropertyType::Matrix4:
				{
					auto mat4Param = static_cast<MaterialMatrix4Property*>(matProperty.get());
					if (mat4Param != nullptr)
					{
						auto mat4 = mat4Param->GetValue();
						glUniformMatrix4fv(uniforms[i].m_location, 1, false, glm::value_ptr(mat4));
					}
					break;
				}
				case MaterialPropertyType::Texture:
				{
					auto textureParam = static_cast<MaterialTextureProperty*>(matProperty.get());
					if (textureParam != nullptr)
					{
						auto texture = textureParam->GetTexture();
						auto textureIdentifierIter = textureResources.find(texture->getInstanceId());
						if (textureIdentifierIter != textureResources.end())
						{
							auto textureIdentifier = textureIdentifierIter->second;
							auto target = textureType2TextureTarget(textureIdentifier.m_textureType);
							if (target != GL_NONE)
							{
								glBindTextureUnit(texCount++, textureIdentifier.m_texture);
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
						Shader::setGlobalBool(uniform.m_uniformName.c_str(), boolParam->GetValue());
					}
					break;
				}
				case MaterialPropertyType::Int:
				{
					auto intParam = static_cast<MaterialIntProperty*>(matProperty.get());
					if (intParam != nullptr)
					{
						Shader::setGlobalInt(uniform.m_uniformName.c_str(), intParam->GetValue());
					}
					break;
				}
				case MaterialPropertyType::Float:
				{
					auto floatParam = static_cast<MaterialFloatProperty*>(matProperty.get());
					if (floatParam != nullptr)
					{
						Shader::setGlobalFloat(uniform.m_uniformName.c_str(), floatParam->GetValue());
					}
					break;
				}
				case MaterialPropertyType::Vector4:
				{
					auto vec4Param = static_cast<MaterialVector4Property*>(matProperty.get());
					if (vec4Param != nullptr)
					{
						Shader::setGlobalVector(uniform.m_uniformName.c_str(), vec4Param->GetValue());
					}
					break;
				}
				case MaterialPropertyType::Matrix4:
				{
					auto mat4Param = static_cast<MaterialMatrix4Property*>(matProperty.get());
					if (mat4Param != nullptr)
					{
						Shader::setGlobalMatrix(uniform.m_uniformName.c_str(), mat4Param->GetValue());
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
	// ����ָ��
	glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_SHORT, NULL);
}