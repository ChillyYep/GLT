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

	void AddBlock(ShaderUniformBlockProperty& block);

	bool HasBlock(std::string name);

	bool FindBlock(std::string blockName, ShaderUniformBlockProperty*& block);

	bool FindBlockByPropertyName(std::string propertyName, ShaderUniformBlockProperty& block) const;

	ShaderUniformBlockProperty* GetBlock(std::string blockName);

	void ReCalculateBufferSizeAndBlockOffset();

	inline std::vector<ShaderUniformBlockProperty>& GetBlocks() { return m_globalBuffer; }

	inline GLTUInt32 GetTotalBufferSize() { return m_bufferSize; }

	static ConstantBufferIdentifier& Null() { return s_null; }

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
