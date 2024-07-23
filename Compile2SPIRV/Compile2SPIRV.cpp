// Compile2SPIRV.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;
const string ShaderDir = R"(E:\Cpp\GLT\GLT\Shader)";
int main()
{
	vector<string> fileList;
	try {
		if (fs::exists(ShaderDir) && fs::is_directory(ShaderDir))
		{
			for (const auto& entry : fs::directory_iterator(ShaderDir))
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
		cerr << e.what() << endl;
	}

	char buffer[1024];
	for (const string& filename : fileList)
	{
		memset(buffer, 0, 1024);
		snprintf(buffer, 1024, R"(E:\Tools\glslang\bin\glslangValidator -V %s\%s -o %s\SPIRV\%s)",
			ShaderDir.c_str(), filename.c_str(), ShaderDir.c_str(), filename.c_str());
		system(buffer);
	}
	std::cout << "Success!\n";
}