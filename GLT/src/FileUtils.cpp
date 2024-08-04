#include "FileUtils.h"
char FileUtils::FilePathBuffer[FilePathBufferLength];

std::string FileUtils::s_globalTextBuffer;

size_t FileUtils::OpenFile(const char* directory, const char* filename, std::string& content)
{
	// 拼接路径
	memset(FilePathBuffer, 0, FilePathBufferLength);
	strcat_s(FilePathBuffer, directory);
	strcat_s(FilePathBuffer, filename);
	FILE* f;
	errno_t err = fopen_s(&f, FilePathBuffer, "rb");
	size_t realFileSize = 0;
	if (err == 0)
	{
		size_t fileSize = 0;
		if (fseek(f, 0, SEEK_END) == 0)
		{
			fileSize = (size_t)ftell(f);
		}
		if (s_globalTextBuffer.size() < fileSize)
		{
			s_globalTextBuffer.resize(fileSize);
		}
		s_globalTextBuffer.clear();
		fseek(f, 0, SEEK_SET);
		// 读取文件
		if (fileSize > 0)
		{
			realFileSize = fread_s(s_globalTextBuffer.data(), s_globalTextBuffer.size(), 1, fileSize, f);
		}
		// 从缓冲中拷贝
		content = s_globalTextBuffer.substr(0, realFileSize);
	}
	if (f != NULL)
	{
		fclose(f);
	}
	return realFileSize;
}
