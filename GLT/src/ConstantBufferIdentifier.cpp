#include "ConstantBufferIdentifier.h"

ConstantBufferIdentifier ConstantBufferIdentifier::s_null;

void ConstantBufferIdentifier::addBlock(ShaderUniformBlockProperty& block)
{
	m_globalBuffer.push_back(block);

	auto lastIndex = m_globalBuffer.size() - 1;

	for (int i = 0;i < block.m_blockUniforms.size();++i)
	{
		m_property2Block[block.m_blockUniforms[i].m_uniformName] = (int)lastIndex;
	}
}

bool ConstantBufferIdentifier::hasBlock(std::string name)
{
	for (int i = 0;i < m_globalBuffer.size();++i)
	{
		if (m_globalBuffer[i].m_name == name)
		{
			return true;
		}
	}
	return false;
}

bool ConstantBufferIdentifier::findBlock(std::string blockName, ShaderUniformBlockProperty*& block)
{
	for (int i = 0;i < m_globalBuffer.size();++i)
	{
		if (m_globalBuffer[i].m_name == blockName)
		{
			block = &m_globalBuffer[i];
			return true;
		}
	}
	return false;
}

bool ConstantBufferIdentifier::findBlockByPropertyName(std::string propertyName, ShaderUniformBlockProperty& block) const
{
	auto blockIter = m_property2Block.find(propertyName);
	if (blockIter == m_property2Block.end())
	{
		block = ShaderUniformBlockProperty::null();
		return false;
	}
	auto bufferIndex = blockIter->second;
	block = m_globalBuffer[bufferIndex];
	return true;
}

ShaderUniformBlockProperty* ConstantBufferIdentifier::getBlock(std::string blockName)
{
	for (int i = 0;i < m_globalBuffer.size();++i)
	{
		if (m_globalBuffer[i].m_name == blockName)
		{
			return &m_globalBuffer[i];
		}
	}
	return nullptr;
}

void ConstantBufferIdentifier::recalculateBufferSizeAndBlockOffset()
{
	m_bufferSize = 0;
	GLTSizei globalBufferCount = (GLTSizei)m_globalBuffer.size();
	for (size_t i = 0;i < globalBufferCount;++i)
	{
		m_globalBuffer[i].m_blockOffset = m_bufferSize;
		m_bufferSize += m_globalBuffer[i].m_preDefineSize;
	}
}