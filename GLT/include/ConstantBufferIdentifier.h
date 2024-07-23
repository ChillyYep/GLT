#pragma once
#include <CommonDefine.h>
#include <unordered_map>
#include <string>
#include <ResourceIdentifier.h>

struct ShaderUniformProperty {
	std::string m_name;
	GLuint m_location;
	GLint m_type;
	GLint m_offset;
	size_t m_size;

	bool operator==(const ShaderUniformProperty& other) const;

	bool operator!=(const ShaderUniformProperty& other) const;
};

struct BlockUniform
{
	std::string m_uniformName;
	GLint m_offset;
	size_t m_size;
	GLint m_type;

	bool operator==(const BlockUniform& other) const;
	bool operator!=(const BlockUniform& other) const;
	inline static BlockUniform Null() { return s_null; }
private:
	static BlockUniform s_null;
};

struct ShaderUniformBlockProperty {
	std::string m_name;

	ConstantBufferType m_constantBufferType;

	GLint m_blockBindingNum;

	/// <summary>
	/// block在整个UBO中的起始位置
	/// </summary>
	GLuint m_blockOffset;

	/// <summary>
	/// 预定义尺寸
	/// </summary>
	GLuint m_preDefineSize;

	std::vector<BlockUniform> m_blockUniforms;

	BlockUniform FindBlockUniformByPropertyName(std::string name);
	bool operator==(const ShaderUniformBlockProperty& other) const;

	bool operator!=(const ShaderUniformBlockProperty& other) const;

	inline static ShaderUniformBlockProperty Null() { return s_null; }
private:
	static ShaderUniformBlockProperty s_null;
};

struct ShaderUniformBlockReference {
	std::string m_uniformBlockName;
	GLuint m_blockIndex;
};

class ConstantBufferIdentifier :public ResourceIdentifier
{
public:
	ConstantBufferIdentifier() {}
	~ConstantBufferIdentifier() {}

	void AddBlock(ShaderUniformBlockProperty& block)
	{
		m_globalBuffer.push_back(block);

		auto lastIndex = m_globalBuffer.size() - 1;

		for (int i = 0;i < block.m_blockUniforms.size();++i)
		{
			m_property2Block[block.m_blockUniforms[i].m_uniformName] = (int)lastIndex;
		}
	}

	bool HasBlock(std::string name)
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

	bool FindBlock(std::string blockName, ShaderUniformBlockProperty*& block)
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

	bool FindBlockByPropertyName(std::string propertyName, ShaderUniformBlockProperty& block) const
	{
		auto blockIter = m_property2Block.find(propertyName);
		if (blockIter == m_property2Block.end())
		{
			block = ShaderUniformBlockProperty::Null();
			return false;
		}
		auto bufferIndex = blockIter->second;
		block = m_globalBuffer[bufferIndex];
		return true;
	}

	ShaderUniformBlockProperty* GetBlock(std::string blockName)
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

	std::vector<ShaderUniformBlockProperty>& GetBlocks() { return m_globalBuffer; }

	inline void ReCalculateBufferSizeAndBlockOffset()
	{
		m_bufferSize = 0;
		GLsizei globalBufferCount = (GLsizei)m_globalBuffer.size();
		for (size_t i = 0;i < globalBufferCount;++i)
		{
			m_globalBuffer[i].m_blockOffset = m_bufferSize;
			m_bufferSize += m_globalBuffer[i].m_preDefineSize;
		}
	}

	inline GLuint GetTotalBufferSize() { return m_bufferSize; }

	static ConstantBufferIdentifier& Null() { return s_null; }

	__GET_SET_PROPERTY__(Ubo, GLuint, m_ubo)
		__GET_SET_PROPERTY__(ConstantBufferType, ConstantBufferType, m_constantBufferType)
private:
	static ConstantBufferIdentifier s_null;
	ConstantBufferType m_constantBufferType;

	std::vector<ShaderUniformBlockProperty> m_globalBuffer;

	std::unordered_map<std::string, int> m_property2Block;

	GLuint m_bufferSize;

	GLuint m_ubo;
};
