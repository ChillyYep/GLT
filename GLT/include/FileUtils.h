#pragma once
#include <cstdio>
#include <string>
class FileUtils
{
public:
	FileUtils() {}
	~FileUtils() {}

	size_t openFile(const char* directory, const char* filename, std::string& content);
private:
	static const int FilePathBufferLength = 256;

	static char s_filePathBuffer[FilePathBufferLength];

	static std::string s_globalTextBuffer;
};