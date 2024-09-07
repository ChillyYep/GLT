#include "ShaderUtils.h"

ShaderUtils::ShaderUtils() {}
ShaderUtils::~ShaderUtils() {}

void ShaderUtils::loadAllShader(ScriptableRenderContext& renderContext)
{
	auto files = ShaderUtils::collectAllFiles();
	GLSLPreProcessCompiler preProcessCompiler;
	auto rawShaderInfos = preProcessCompiler.parse(files);
	// 编译Shader
	GLSLCompiler compiler;
	CompiledResult compileResult;
	compiler.compileShaderProgram(rawShaderInfos, compileResult);
	// 收集编译结果
	renderContext.requestConstantBufferResources(compileResult.constantBuffers);
	Shader::init(compileResult.outputShaderPrograms, compileResult.constantBuffers);
}

void ShaderUtils::unloadAllShader(ScriptableRenderContext& renderContext)
{
	auto& identifierMap = Shader::getShaderConstantBufferSet().getAllConstantBufferIdentifier();
	std::vector<ConstantBufferIdentifier> identifiers;

	for (const auto& identifierPair : identifierMap)
	{
		identifiers.push_back(identifierPair.second);
	}
	renderContext.destroyConstantBufferResources(identifiers);

}

std::vector<std::string> ShaderUtils::collectAllFiles()
{
	std::vector<std::string> fileList;
	try {
		if (fs::exists(SHADER_ROOT) && fs::is_directory(SHADER_ROOT))
		{
			for (const auto& entry : fs::directory_iterator(SHADER_ROOT))
			{
				if (fs::is_regular_file(entry))
				{
					fileList.push_back(entry.path().filename().string());
				}
			}
		}
	}
	catch (const fs::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return fileList;
}
