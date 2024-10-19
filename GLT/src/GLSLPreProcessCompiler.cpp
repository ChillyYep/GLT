#include "GLSLPreProcessCompiler.h"

std::unordered_map<std::string, RawShaderInfo> GLSLPreProcessCompiler::parse(std::vector<std::string>& fileList) {
	std::unordered_map<std::string, RawShaderInfo> map;
	std::stringstream ss;
	std::ifstream fileStream;

	for (int i = 0;i < fileList.size();++i)
	{
		const auto shaderFileName = fileList[i];
		auto index = shaderFileName.find_last_of('.');
		auto suffix = shaderFileName.substr(index + 1);
		auto filename = shaderFileName.substr(0, index);
		std::string line;
		ss.clear();
		ss.str("");
		if (suffix == "vert" || suffix == "frag")
		{
			auto shaderFilePath = SHADER_ROOT + shaderFileName;
			fileStream.open(shaderFilePath, std::ios::out);
			if (fileStream.is_open())
			{
				while (std::getline(fileStream, line))
				{
					auto trimLine = StringUtility::Trim(line);
					if (StringUtility::StartWith(trimLine, "#include"))
					{
						auto includeFile = extractIncludeFile(trimLine, "#include");
						auto includeFilePath = SHADER_ROOT + includeFile;
						assert(fs::exists(includeFilePath) && !fs::is_directory(includeFilePath));

						auto text = FileUtils::readAllText(includeFilePath);
						ss << text;
					}
					else {
						ss << line << std::endl;
					}
				}
			}
			fileStream.close();

			RawShaderInfo rawShaderInfo;
			if (map.find(filename) == map.end())
			{
				rawShaderInfo.m_programName = filename;
				map[filename] = rawShaderInfo;
			}
			if (suffix == "vert")
			{
				map[filename].m_vertexShader = ss.str();
			}
			else {
				map[filename].m_fragmentShader = ss.str();
			}
		}
	}

	//for (const auto& pair : map)
	//{
	//	std::cout << pair.first << ".vert" << std::endl;
	//	std::cout << pair.second.m_vertexShader << std::endl;
	//	std::cout << pair.first << ".frag" << std::endl;
	//	std::cout << pair.second.m_fragmentShader << std::endl;
	//}

	return map;
}

std::string GLSLPreProcessCompiler::extractIncludeFile(const std::string& line, const std::string& pattern)
{
	if (line.size() < pattern.size())
	{
		return "";
	}
	std::stringstream ss;
	bool start = false;
	for (size_t i = pattern.size();i < line.size();++i)
	{
		if (line[i] == '\"')
		{
			start = true;
		}
		else if (start && line[i] != '\"')
		{
			ss << line[i];
		}
	}
	return ss.str();
}