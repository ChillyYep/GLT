#pragma once
#include <CommonDefine.h>
#include <unordered_map>
#include <string>
#include <ResourceIdentifier.h>
#include <RHICommon.h>

class ConstantBufferIdentifier :public ResourceIdentifier
{
public:
	ConstantBufferIdentifier() {}
	~ConstantBufferIdentifier() {}

	void addBlock(ShaderUniformBlockProperty& block);

	bool hasBlock(std::string name);

	bool findBlock(std::string blockName, ShaderUniformBlockProperty*& block);

	bool findBlockByPropertyName(std::string propertyName, ShaderUniformBlockProperty& block) const;

	ShaderUniformBlockProperty* getBlock(std::string blockName);

	void recalculateBufferSizeAndBlockOffset();

	inline std::vector<ShaderUniformBlockProperty>& getBlocks() { return m_globalBuffer; }

	inline GLTUInt32 getTotalBufferSize() { return m_bufferSize; }

	static ConstantBufferIdentifier& null() { return s_null; }

	__GET_SET_PROPERTY__(Ubo, GLTUInt32, m_ubo)
		__GET_SET_PROPERTY__(ConstantBufferType, ConstantBufferType, m_constantBufferType)
private:
	static ConstantBufferIdentifier s_null;

	ConstantBufferType m_constantBufferType;

	std::vector<ShaderUniformBlockProperty> m_globalBuffer;

	std::unordered_map<std::string, int> m_property2Block;

	GLTUInt32 m_bufferSize;

	GLTUInt32 m_ubo;
};
