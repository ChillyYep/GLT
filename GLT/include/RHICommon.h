#pragma once
#include <CommonDefine.h>
#include <string>
#include <vector>
#include <ConstantBufferType.h>

struct ShaderUniformProperty {
	std::string m_name;
	GLTUInt32 m_location;
	GLTInt32 m_type;
	GLTInt32 m_offset;
	size_t m_size;

	bool operator==(const ShaderUniformProperty& other) const;

	bool operator!=(const ShaderUniformProperty& other) const;
};

struct BlockUniform
{
	std::string m_uniformName;
	GLTInt32 m_offset;
	size_t m_size;
	GLTInt32 m_type;

	bool operator==(const BlockUniform& other) const;
	bool operator!=(const BlockUniform& other) const;
	inline static BlockUniform null() { return s_null; }
private:
	static BlockUniform s_null;
};

struct ShaderUniformBlockProperty {
	std::string m_name;

	ConstantBufferType m_constantBufferType;

	GLTInt32 m_blockBindingNum;

	/// <summary>
	/// block在整个UBO中的起始位置
	/// </summary>
	GLTUInt32 m_blockOffset;

	/// <summary>
	/// 预定义尺寸
	/// </summary>
	GLTUInt32 m_preDefineSize;

	std::vector<BlockUniform> m_blockUniforms;

	BlockUniform findBlockUniformByPropertyName(std::string name);
	bool operator==(const ShaderUniformBlockProperty& other) const;

	bool operator!=(const ShaderUniformBlockProperty& other) const;

	inline static ShaderUniformBlockProperty null() { return s_null; }
private:
	static ShaderUniformBlockProperty s_null;
};

struct ShaderUniformBlockReference {
	std::string m_uniformBlockName;
	GLTUInt32 m_blockIndex;
};
struct ShaderProgramInfo
{
	std::string m_programName;
	GLTUInt32 m_program;
	std::vector<ShaderUniformProperty> m_uniformProperties;
	std::vector<ShaderUniformBlockReference> m_referenceBlocks;
};

struct RawShaderInfo
{
	std::string m_programName;
	std::string m_vertexShader;
	std::string m_fragmentShader;
};
