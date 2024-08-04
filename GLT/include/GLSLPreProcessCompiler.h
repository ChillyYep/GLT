#pragma once
#include <CommonDefine.h>
#include <ShaderPreProcessCompilerBase.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <StringUtility.h>
#include <FileUtils.h>

class GLSLPreProcessCompiler :public ShaderPreProcessCompilerBase
{
public:
	GLSLPreProcessCompiler() {}
	~GLSLPreProcessCompiler() {}

	std::unordered_map<std::string, RawShaderInfo> parse(std::vector<std::string>& fileList) override;
private:
	std::string extractIncludeFile(const std::string& line, const std::string& pattern);
};