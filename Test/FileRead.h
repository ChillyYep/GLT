#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
namespace fs = std::filesystem;

struct RawShaderInfo
{
	std::string m_programName;
	std::string m_vertexShader;
	std::string m_fragmentShader;
};

class FileUtils
{
public:
	FileUtils();
	~FileUtils();
	static const std::string SHADER_ROOT;

	static void CollectAllFiles()
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
		std::unordered_map<std::string, RawShaderInfo> map;
		std::stringstream ss;
		std::ifstream fileStream;
		std::string vertexShader;
		std::string fragmentShader;

		for (int i = 0;i < fileList.size();++i)
		{
			const auto shaderFileName = fileList[i];
			auto index = shaderFileName.find_last_of('.');
			auto suffix = shaderFileName.substr(index + 1);
			auto filename = shaderFileName.substr(0, index);
			std::string line;
			ss.clear();
			vertexShader = "";
			if (suffix == "vert")
			{
				auto shaderFilePath = SHADER_ROOT + shaderFileName;
				fileStream.open(shaderFilePath, std::ios::out);
				if (fileStream.is_open())
				{
					while (std::getline(fileStream, line))
					{
						auto trimLine = Trim(line);
						if (StartWith(trimLine, "#include"))
						{
							auto includeFile = ExtractIncludeFile(trimLine, "#include");
							auto includeFilePath = SHADER_ROOT + includeFile;
							assert(fs::exists(includeFilePath) && !fs::is_directory(includeFilePath));

							auto text = ReadAllText(includeFilePath);
							ss << text;
						}
						else {
							ss << line << std::endl;
						}
					}
				}
				fileStream.close();

				vertexShader = ss.str();

				RawShaderInfo rawShaderInfo;
				if (map.find(filename) == map.end())
				{
					rawShaderInfo.m_programName = filename;
					map[filename] = rawShaderInfo;
				}

				map[filename].m_vertexShader = vertexShader;
			}
		}

		for (const auto& pair : map)
		{
			std::cout << pair.first << std::endl;
			std::cout << pair.second.m_vertexShader << std::endl;
			std::cout << pair.second.m_fragmentShader << std::endl;
		}
	}
private:
	static std::string ReadAllText(std::string& filePath)
	{
		std::ifstream fileStream;
		std::stringstream ss;
		fileStream.open(filePath, std::ios::out);
		if (fileStream.is_open())
		{
			std::string line;
			while (std::getline(fileStream, line))
			{
				ss << line << std::endl;
			}
		}
		fileStream.close();
		return ss.str();
	}

	static std::string Trim(const std::string& line)
	{
		std::stringstream ss;
		for (int i = 0;i < line.size();++i)
		{
			if ((line[i] >= '\t' && line[i] <= '\r') || line[i] == ' ')
			{
				continue;
			}
			else {
				ss << line[i];
			}
		}
		return ss.str();
	}

	static bool StartWith(const std::string& line, const std::string& pattern)
	{
		if (line.size() < pattern.size())
		{
			return false;
		}
		for (int i = 0;i < pattern.size() && i < line.size();++i)
		{
			if (line[i] != pattern[i])
			{
				return false;
			}
		}
		return true;
	}

	static std::string ExtractIncludeFile(const std::string& line, const std::string& pattern)
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
};

const std::string FileUtils::SHADER_ROOT = "../GLT/Shader/";
FileUtils::FileUtils()
{
}

FileUtils::~FileUtils()
{
}