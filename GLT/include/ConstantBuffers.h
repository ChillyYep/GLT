#pragma once
#include <ConstantBufferIdentifier.h>
#include <BytesBuffer.h>
#include <unordered_map>
#include <GLCommon.h>

class ConstantBufferSet
{
public:
	ConstantBufferSet() {}
	~ConstantBufferSet() {}

	void create(ConstantBufferIdentifier globalBufferIdentifier)
	{
		auto shaderGlobalType = globalBufferIdentifier.GetConstantBufferType();
		if (m_globalBufferIdentifiers.find(shaderGlobalType) != m_globalBufferIdentifiers.end())
		{
			return;
		}
		GLuint bufferSize = globalBufferIdentifier.getTotalBufferSize();
		if (bufferSize > 0)
		{
			m_globalBufferIdentifiers[shaderGlobalType] = globalBufferIdentifier;

			m_bytesBuffers[shaderGlobalType] = BytesBuffer();
			m_bytesBuffers[shaderGlobalType].create(bufferSize);
		}
	}

	void setData(ConstantBufferType bufferType, GLuint offset, GLuint length, void* data)
	{
		if (m_bytesBuffers.find(bufferType) == m_bytesBuffers.end())
		{
			return;
		}
		m_bytesBuffers[bufferType].setData(offset, length, data);
	}

	void upload(ConstantBufferType constantBufferType)
	{
		if (m_globalBufferIdentifiers.find(constantBufferType) != m_globalBufferIdentifiers.end())
		{
			auto identifier = m_globalBufferIdentifiers[constantBufferType];
			auto bytesBuffer = m_bytesBuffers[constantBufferType];
			GLuint ubo = identifier.GetUbo();
			glNamedBufferSubData(ubo, 0, bytesBuffer.dataSize(), bytesBuffer.data());
		}
	}

	void findProperty(std::string propertyName, ShaderUniformBlockProperty& block, BlockUniform& blockUniform, ConstantBufferType& constantBufferType)
	{
		for (auto& bufferIdentifier : m_globalBufferIdentifiers)
		{
			if (bufferIdentifier.second.findBlockByPropertyName(propertyName, block))
			{
				constantBufferType = bufferIdentifier.first;
				blockUniform = block.findBlockUniformByPropertyName(propertyName);
				return;
			}
			else {
				blockUniform = BlockUniform::null();
			}
		}
	}

	void getPropertyRange(ShaderUniformBlockProperty& block, BlockUniform& blockUniform, GLuint* offset, GLuint* length)
	{
		*offset = block.m_blockOffset + blockUniform.m_offset;
		*length = (GLuint)blockUniform.m_size;
	}

	inline ShaderUniformBlockProperty* findBlock(std::string blockName)
	{
		for (auto& identifierPair : m_globalBufferIdentifiers)
		{
			auto constantBufferType = identifierPair.first;
			auto identifier = identifierPair.second;
			ShaderUniformBlockProperty* block;
			if (identifier.findBlock(blockName, block))
			{
				return block;
			}
		}
		return nullptr;
	}

	bool hasConstantBuffer(ConstantBufferType constantBufferType)
	{
		return m_globalBufferIdentifiers.find(constantBufferType) != m_globalBufferIdentifiers.end();
	}

	inline ConstantBufferIdentifier& getGlobalBufferIdentifier(ConstantBufferType bufferType)
	{
		return m_globalBufferIdentifiers.find(bufferType) == m_globalBufferIdentifiers.end() ? ConstantBufferIdentifier::null() : m_globalBufferIdentifiers[bufferType];
	}

	inline ConstantBufferIdentifier& getGlobalBufferIdentifierByBlockName(std::string blockName)
	{
		for (auto& identifierPair : m_globalBufferIdentifiers)
		{
			auto constantBufferType = identifierPair.first;
			auto identifier = identifierPair.second;
			ShaderUniformBlockProperty* block;
			if (identifier.findBlock(blockName, block))
			{
				return m_globalBufferIdentifiers[constantBufferType];
			}
		}
		return ConstantBufferIdentifier::null();
	}
private:
	std::unordered_map<ConstantBufferType, ConstantBufferIdentifier> m_globalBufferIdentifiers;

	std::unordered_map<ConstantBufferType, BytesBuffer> m_bytesBuffers;
};