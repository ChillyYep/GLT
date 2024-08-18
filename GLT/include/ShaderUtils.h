#pragma once
#include <CommonDefine.h>
#include <ResourceManager.h>
#include <Shader.h>
#include <vector>
#include <StringUtility.h>
#include <ScriptableRenderContext.h>
#include <GLSLPreProcessCompiler.h>
#include <GLSLCompiler.h>
namespace fs = std::filesystem;

class ShaderUtils
{
public:
	ShaderUtils();
	~ShaderUtils();
	static std::vector<std::string> collectAllFiles();

	static void loadAllShader(ScriptableRenderContext& renderContext);
	static void unloadAllShader(ScriptableRenderContext& renderContext);
private:

	static char infoLog[512];
};