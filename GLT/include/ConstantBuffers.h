#pragma once
#include <ConstantBufferIdentifier.h>
#include <BytesBuffer.h>

class ConstantBufferSet
{
public:
	ConstantBufferSet() {}
	~ConstantBufferSet() {}

	void Create(ConstantBufferIdentifier globalBufferIdentifier)
	{
		auto shaderGlobalType = globalBufferIdentifier.GetConstantBufferType();
		if (m_globalBufferIdentifiers.find(shaderGlobalType) != m_globalBufferIdentifiers.end())
		{
			return;
		}
		GLuint bufferSize = globalBufferIdentifier.GetTotalBufferSize();
		if (bufferSize > 0)
		{
			m_globalBufferIdentifiers[shaderGlobalType] = globalBufferIdentifier;

			m_bytesBuffers[shaderGlobalType] = BytesBuffer();
			m_bytesBuffers[shaderGlobalType].Create(bufferSize);
		}
	}

	void SetData(ConstantBufferType bufferType, GLuint offset, GLuint length, void* data)
	{
		if (m_bytesBuffers.find(bufferType) == m_bytesBuffers.end())
		{
			return;
		}
		m_bytesBuffers[bufferType].SetData(offset, length, data);
	}

	void Upload(ConstantBufferType constantBufferType)
	{
		if (m_globalBufferIdentifiers.find(constantBufferType) != m_globalBufferIdentifiers.end())
		{
			auto identifier = m_globalBufferIdentifiers[constantBufferType];
			auto bytesBuffer = m_bytesBuffers[constantBufferType];
			GLuint ubo = identifier.GetUbo();
			glNamedBufferSubData(ubo, 0, bytesBuffer.dataSize(), bytesBuffer.data());
		}
	}

	void FindProperty(std::string propertyName, ShaderUniformBlockProperty& block, BlockUniform& blockUniform, ConstantBufferType& constantBufferType)
	{
		for (auto& bufferIdentifier : m_globalBufferIdentifiers)
		{
			if (bufferIdentifier.second.FindBlockByPropertyName(propertyName, block))
			{
				constantBufferType = bufferIdentifier.first;
				blockUniform = block.FindBlockUniformByPropertyName(propertyName);
				return;
			}
			else {
				blockUniform = BlockUniform::Null();
			}
		}
	}

	void GetPropertyRange(ShaderUniformBlockProperty& block, BlockUniform& blockUniform, GLuint* offset, GLuint* length)
	{
		*offset = block.m_blockOffset + blockUniform.m_offset;
		*length = (GLuint)blockUniform.m_size;
	}

	inline ShaderUniformBlockProperty* FindBlock(std::string blockName)
	{
		for (auto& identifierPair : m_globalBufferIdentifiers)
		{
			auto constantBufferType = identifierPair.first;
			auto identifier = identifierPair.second;
			ShaderUniformBlockProperty* block;
			if (identifier.FindBlock(blockName, block))
			{
				return block;
			}
		}
		return nullptr;
	}

	bool HasConstantBuffer(ConstantBufferType constantBufferType)
	{
		return m_globalBufferIdentifiers.find(constantBufferType) != m_globalBufferIdentifiers.end();
	}

	inline ConstantBufferIdentifier& GetGlobalBufferIdentifier(ConstantBufferType bufferType)
	{
		return m_globalBufferIdentifiers.find(bufferType) == m_globalBufferIdentifiers.end() ? ConstantBufferIdentifier::Null() : m_globalBufferIdentifiers[bufferType];
	}

	inline ConstantBufferIdentifier& GetGlobalBufferIdentifierByBlockName(std::string blockName)
	{
		for (auto& identifierPair : m_globalBufferIdentifiers)
		{
			auto constantBufferType = identifierPair.first;
			auto identifier = identifierPair.second;
			ShaderUniformBlockProperty* block;
			if (identifier.FindBlock(blockName, block))
			{
				return m_globalBufferIdentifiers[constantBufferType];
			}
		}
		return ConstantBufferIdentifier::Null();
	}
private:
	std::unordered_map<ConstantBufferType, ConstantBufferIdentifier> m_globalBufferIdentifiers;

	std::unordered_map<ConstantBufferType, BytesBuffer> m_bytesBuffers;
};