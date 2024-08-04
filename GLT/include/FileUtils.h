#pragma once
#include <cstdio>
#include <string>
class FileUtils
{
public:
	FileUtils() {}
	~FileUtils() {}

	size_t OpenFile(const char* directory, const char* filename, std::string& content);
private:
	static const int FilePathBufferLength = 256;

	static char FilePathBuffer[FilePathBufferLength];

	static std::string s_globalTextBuffer;
};