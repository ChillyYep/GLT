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

	void create(ConstantBufferIdentifier globalBufferIdentifier);

	inline void setData(ConstantBufferType bufferType, GLuint offset, GLuint length, void* data)
	{
		if (m_bytesBuffers.find(bufferType) == m_bytesBuffers.end())
		{
			return;
		}
		m_bytesBuffers[bufferType].setData(offset, length, data);
	}

	void findProperty(std::string propertyName, ShaderUniformBlockProperty& block, BlockUniform& blockUniform, ConstantBufferType& constantBufferType);

	void getPropertyRange(ShaderUniformBlockProperty& block, BlockUniform& blockUniform, GLuint* offset, GLuint* length)
	{
		*offset = block.m_blockOffset + blockUniform.m_offset;
		*length = (GLuint)blockUniform.m_size;
	}

	ShaderUniformBlockProperty* findBlock(std::string blockName);

	inline bool hasConstantBuffer(ConstantBufferType constantBufferType)
	{
		return m_globalBufferIdentifiers.find(constantBufferType) != m_globalBufferIdentifiers.end();
	}

	inline ConstantBufferIdentifier* getGlobalBufferIdentifier(ConstantBufferType bufferType)
	{
		return m_globalBufferIdentifiers.find(bufferType) == m_globalBufferIdentifiers.end() ? nullptr : &m_globalBufferIdentifiers[bufferType];
	}
	inline BytesBuffer* getGlobalByteBuffer(ConstantBufferType bufferType)
	{
		return m_bytesBuffers.find(bufferType) == m_bytesBuffers.end() ? nullptr : &m_bytesBuffers[bufferType];
	}
	
	ConstantBufferIdentifier* getGlobalBufferIdentifierByBlockName(std::string blockName);
	
	inline std::unordered_map<ConstantBufferType, ConstantBufferIdentifier>& getAllConstantBufferIdentifier()
	{
		return m_globalBufferIdentifiers;
	}
private:
	std::unordered_map<ConstantBufferType, ConstantBufferIdentifier> m_globalBufferIdentifiers;

	std::unordered_map<ConstantBufferType, BytesBuffer> m_bytesBuffers;
};