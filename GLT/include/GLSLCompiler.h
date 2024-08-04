#pragma once
#include <CommonDefine.h>
#include <GLCommon.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <ConstantBufferIdentifier.h>
#include <RHICommon.h>
#include <assert.h>
#include <StringUtility.h>

struct CompiledResult {
	std::unordered_map<std::string, ShaderProgramInfo> outputShaderPrograms;
	std::vector<ConstantBufferIdentifier> constantBuffers;
};

class GLSLCompiler
{
public:
	GLSLCompiler() {}
	~GLSLCompiler() {}

	void compileShaderProgram(std::unordered_map<std::string, RawShaderInfo>& rawShaderInfo, CompiledResult& compiledResult);
private:
	GLuint compileShader(std::string shaderName, GLuint shaderStage, const char* shaderCode, GLint shaderCodeLength);

	GLuint linkProgram(std::string programName, GLuint vertexShader, GLuint fragmentShader);

	void afterEveryProgramCompiled(GLuint program, ShaderProgramInfo& shaderProgramInfo, std::vector<ConstantBufferIdentifier>& constantBuffers);

	void afterAllProgramCompiled(const std::unordered_map<std::string, ShaderProgramInfo>& outputShaderPrograms, std::vector<ConstantBufferIdentifier>& constantBuffers);
	
	void extractBlocks(GLuint program, GLint numBlocks, GLint maxBlockNameLength, const std::vector<std::string>& uniformNames, const std::vector<GLuint>& uniformIndcies,
		const std::vector<GLint>& uniformSizes, const std::vector<GLint>& uniformOffsets, const std::vector<GLint>& uniformTypes,
		std::vector<ConstantBufferIdentifier>& constantBuffers, std::vector<ShaderUniformBlockReference>& blockReferences);

	void extractNoBlockUniforms(GLuint program, GLint numProperties, GLint maxPropertyNameLength, const std::vector<std::string>& uniformNames, const std::vector<GLint>& uniformSizes,
		const std::vector<GLint>& uniformOffsets, const std::vector<GLint>& uniformTypes, std::vector<ShaderUniformProperty>& uniformProperties);

	void extractUniformIndices(GLuint program, std::vector<std::string>& uniformNames, std::vector<GLuint>& uniformIndcies, std::vector<GLint>& uniformSizes,
		std::vector<GLint>& uniformOffsets, std::vector<GLint>& uniformTypes);

	std::vector<std::string> extractUniformNames(GLuint program, GLint numProperties, GLint maxPropertyNameLength);

	void getProgramInfo(GLuint program, GLint* maxPropertyNameLength, GLint* numProperties, GLint* maxBlockNameLength, GLint* numBlocks);

	static size_t typeSize(GLenum type);

	static bool isTypeSupportBlock(GLenum type);

	GLuint getPreDefineSize(std::vector<BlockUniform>& blockUniforms);

	ShaderUniformBlockProperty* findBlock(std::vector<ConstantBufferIdentifier>& constantBuffers, std::string blockName, int* constantBufferIndex = nullptr);

	int getUniformIndex(GLuint index, const std::vector<std::string>& uniformNames, const std::vector<GLuint>& uniformIndcies);

	ConstantBufferType getConstantType(std::string blockName);

	ConstantBufferIdentifier* getConstantBuffer(std::vector<ConstantBufferIdentifier>& constantBuffers, ConstantBufferType constantBufferType);

	char infoLog[512];
};