#pragma once
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;

class FileUtils
{
public:
	FileUtils() {}
	~FileUtils() {}

	static size_t openFile(const char* directory, const char* filename, std::string& content);

	static std::string readAllText(std::string& filePath);
private:
	static const int FilePathBufferLength = 256;

	static char s_filePathBuffer[FilePathBufferLength];

	static std::string s_globalTextBuffer;
};