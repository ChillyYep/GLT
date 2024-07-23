#pragma once
#include <string.h>
#include <sstream>
class StringUtility
{
public:
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
	static bool EndsWith(std::string& str, std::string suffix)
	{
		size_t suffixSize = suffix.size();
		size_t strSize = str.size();
		if (suffixSize > strSize)
		{
			return false;
		}
		for (int i = 0;i < suffix.size();++i)
		{
			if (str[strSize - 1 - i] != suffix[suffixSize - 1 - i])
			{
				return false;
			}
		}
		return true;
	}
};
