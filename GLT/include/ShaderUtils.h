#pragma once
#include <CommonDefine.h>
#include <ResourceManager.h>
#include <Shader.h>
#include <vector>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <StringUtility.h>
#include <ScriptableRenderContext.h>
namespace fs = std::filesystem;

struct RawShaderInfo
{
	std::string m_programName;
	std::string m_vertexShader;
	std::string m_fragmentShader;
};

class ShaderUtils
{
public:
	ShaderUtils();
	~ShaderUtils();
	static std::vector<std::string> CollectAllFiles();

	static std::unordered_map<std::string, RawShaderInfo> Parse(std::vector<std::string>& fileList);

	static void CompileShaderProgram(ScriptableRenderContext& renderContext, std::unordered_map<std::string, RawShaderInfo>& rawShaderInfo, std::unordered_map<std::string, ShaderProgramInfo>& outputShaderPrograms, std::vector<ConstantBufferIdentifier>& constantBuffers);

	static void LoadAllShader(ScriptableRenderContext& renderContext);

	static bool IsTypeSupportBlock(GLenum type);

	static void BindBlockForProgram(Shader& shader);
private:
	static size_t TypeSize(GLenum type);

	static ConstantBufferType GetConstantType(std::string blockName);

	static ConstantBufferIdentifier* GetConstantBuffer(std::vector<ConstantBufferIdentifier>& constantBuffers, ConstantBufferType constantBufferType);

	static ShaderUniformBlockProperty* FindBlock(std::vector<ConstantBufferIdentifier>& constantBuffers, std::string blockName, int* constantBufferIndex = nullptr);

	static GLuint GetPreDefineSize(std::vector<BlockUniform>& blockUniforms);

	static GLuint CompileShader(ScriptableRenderContext& renderContext, std::string shaderName, GLuint shaderStage, const char* shaderCode, GLint shaderCodeLength);

	static GLuint LinkProgram(ScriptableRenderContext& renderContext, std::string programName, GLuint vertexShader, GLuint fragmentShader);

	static void GetProgramInfo(GLuint program, GLint* maxPropertyNameLength, GLint* numProperties, GLint* maxBlockNameLength, GLint* numBlocks);

	static std::vector<std::string> ExtractUniformNames(GLuint program, GLint numProperties, GLint maxPropertyNameLength);

	static void ExtractUniformIndices(GLuint program, std::vector<std::string>& uniformNames, std::vector<GLuint>& uniformIndcies, std::vector<GLint>& uniformSizes,
		std::vector<GLint>& uniformOffsets, std::vector<GLint>& uniformTypes);

	static int GetUniformIndex(GLuint index, const std::vector<std::string>& uniformNames, const std::vector<GLuint>& uniformIndcies);

	static void ExtractBlocks(GLuint program, GLint numBlocks, GLint maxBlockNameLength, const std::vector<std::string>& uniformNames, const std::vector<GLuint>& uniformIndcies,
		const std::vector<GLint>& uniformSizes, const std::vector<GLint>& uniformOffsets, const std::vector<GLint>& uniformTypes,
		std::vector<ConstantBufferIdentifier>& constantBuffers, std::vector<ShaderUniformBlockReference>& blockReferences);

	static void ExtractNoBlockUniforms(GLuint program, GLint numProperties, GLint maxPropertyNameLength, const std::vector<std::string>& uniformNames, const std::vector<GLint>& uniformSizes,
		const std::vector<GLint>& uniformOffsets, const std::vector<GLint>& uniformTypes, std::vector<ShaderUniformProperty>& uniformProperties);

	static void AfterEveryProgramCompiled(GLuint program, ShaderProgramInfo& shaderProgramInfo, std::vector<ConstantBufferIdentifier>& constantBuffers);

	static void AfterAllProgramCompiled(const std::unordered_map<std::string, ShaderProgramInfo>& outputShaderPrograms, std::vector<ConstantBufferIdentifier>& constantBuffers);

	static std::string ReadAllText(std::string& filePath);

	static std::string ExtractIncludeFile(const std::string& line, const std::string& pattern);

	static char infoLog[512];
};