#pragma once
#include <unordered_map>
#include <string>
#include <RHICommon.h>

class ShaderPreProcessCompilerBase
{
public:
	ShaderPreProcessCompilerBase() {}
	~ShaderPreProcessCompilerBase() {}

	virtual std::unordered_map<std::string, RawShaderInfo> parse(std::vector<std::string>& fileList) = 0;
private:

};