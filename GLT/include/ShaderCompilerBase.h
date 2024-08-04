#pragma once
#include <unordered_map>
#include <vector>

struct CompiledResult {
	std::unordered_map<std::string, ShaderProgramInfo> outputShaderPrograms;
	std::vector<ConstantBufferIdentifier> constantBuffers;
};

class ShaderCompilerBase
{
public:
	ShaderCompilerBase() {}
	~ShaderCompilerBase() {}

	virtual void compileShaderProgram(std::unordered_map<std::string, RawShaderInfo>& rawShaderInfo, CompiledResult& compiledResult) = 0;
private:

};