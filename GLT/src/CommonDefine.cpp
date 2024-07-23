#include "CommonDefine.h"

const int FilePathBufferLength = 256;
char FilePathBuffer[FilePathBufferLength];

const int FileContentBufferLength = 2048;
char FileContentBuffer[FileContentBufferLength];

size_t OpenFile(const char* directory, const char* filename, char*& content)
{
	memset(FilePathBuffer, 0, FilePathBufferLength);
	strcat_s(FilePathBuffer, directory);
	strcat_s(FilePathBuffer, filename);
	FILE* f;
	errno_t err = fopen_s(&f, FilePathBuffer, "rb");
	size_t realFileSize = 0;
	if (err == 0)
	{
		size_t fileSize;
		if (fseek(f, 0, SEEK_END) == 0)
		{
			fileSize = (size_t)ftell(f);
		}
		fseek(f, 0, SEEK_SET);
		memset(FileContentBuffer, 0, FileContentBufferLength);
		realFileSize = fread_s(FileContentBuffer, FileContentBufferLength, 1, fileSize, f);
		content = FileContentBuffer;
	}
	if (f != NULL)
	{
		fclose(f);
	}
	return realFileSize;
}

std::string OpenFile(const char* directory, const char* filename)
{
	memset(FilePathBuffer, 0, FilePathBufferLength);
	strcat_s(FilePathBuffer, directory);
	strcat_s(FilePathBuffer, filename);
	FILE* f;
	errno_t err = fopen_s(&f, FilePathBuffer, "rb");
	size_t realFileSize = 0;
	std::string str;
	if (err == 0)
	{
		size_t fileSize = 0;
		if (fseek(f, 0, SEEK_END) == 0)
		{
			fileSize = (size_t)ftell(f);
		}
		fseek(f, 0, SEEK_SET);
		str = std::string(fileSize, '\0');
		realFileSize = fread_s(str.data(), fileSize, 1, fileSize, f);
	}
	if (f != NULL)
	{
		fclose(f);
	}
	return str;
}
