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
#include <GLSLCompiler.h>
namespace fs = std::filesystem;

class ShaderUtils
{
public:
	ShaderUtils();
	~ShaderUtils();
	static std::vector<std::string> collectAllFiles();

	static std::unordered_map<std::string, RawShaderInfo> parse(std::vector<std::string>& fileList);

	static void loadAllShader(ScriptableRenderContext& renderContext);

	static void bindBlockForProgram(Shader& shader);
private:
	static std::string readAllText(std::string& filePath);

	static std::string extractIncludeFile(const std::string& line, const std::string& pattern);

	static char infoLog[512];
};