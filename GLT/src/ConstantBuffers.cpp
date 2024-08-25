#include "ConstantBuffers.h"

void ConstantBufferSet::create(ConstantBufferIdentifier globalBufferIdentifier)
{
	auto shaderGlobalType = globalBufferIdentifier.getConstantBufferType();
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

void ConstantBufferSet::findProperty(std::string propertyName, ShaderUniformBlockProperty& block, BlockUniform& blockUniform, ConstantBufferType& constantBufferType)
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

ShaderUniformBlockProperty* ConstantBufferSet::findBlock(std::string blockName)
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

ConstantBufferIdentifier* ConstantBufferSet::getGlobalBufferIdentifierByBlockName(std::string blockName)
{
	for (auto& identifierPair : m_globalBufferIdentifiers)
	{
		auto constantBufferType = identifierPair.first;
		auto identifier = identifierPair.second;
		ShaderUniformBlockProperty* block;
		if (identifier.findBlock(blockName, block))
		{
			return &m_globalBufferIdentifiers[constantBufferType];
		}
	}
	return nullptr;
}