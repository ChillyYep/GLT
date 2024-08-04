#include "FileUtils.h"
char FileUtils::s_filePathBuffer[FilePathBufferLength];

std::string FileUtils::s_globalTextBuffer;

size_t FileUtils::openFile(const char* directory, const char* filename, std::string& content)
{
	// ƴ��·��
	memset(s_filePathBuffer, 0, FilePathBufferLength);
	strcat_s(s_filePathBuffer, directory);
	strcat_s(s_filePathBuffer, filename);
	FILE* f;
	errno_t err = fopen_s(&f, s_filePathBuffer, "rb");
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
		// ��ȡ�ļ�
		if (fileSize > 0)
		{
			realFileSize = fread_s(s_globalTextBuffer.data(), s_globalTextBuffer.size(), 1, fileSize, f);
		}
		// �ӻ����п���
		content = s_globalTextBuffer.substr(0, realFileSize);
	}
	if (f != NULL)
	{
		fclose(f);
	}
	return realFileSize;
}


std::string FileUtils::readAllText(std::string& filePath)
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