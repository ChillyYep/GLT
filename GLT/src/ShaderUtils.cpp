#include "ShaderUtils.h"

ShaderUtils::ShaderUtils() {}
ShaderUtils::~ShaderUtils() {}

void ShaderUtils::loadAllShader(ScriptableRenderContext& renderContext)
{
	auto files = ShaderUtils::collectAllFiles();
	auto rawShaderInfos = ShaderUtils::parse(files);
	// 编译Shader
	GLSLCompiler compiler;
	CompiledResult compileResult;
	compiler.compileShaderProgram(rawShaderInfos, compileResult);
	// 收集编译结果
	ResourceManager::getInstance()->requestConstantBufferResource(compileResult.constantBuffers);
	Shader::init(compileResult.outputShaderPrograms, compileResult.constantBuffers);
}

std::vector<std::string> ShaderUtils::collectAllFiles()
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
	return fileList;
}

std::unordered_map<std::string, RawShaderInfo> ShaderUtils::parse(std::vector<std::string>& fileList) {
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

						auto text = readAllText(includeFilePath);
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

	for (const auto& pair : map)
	{
		std::cout << pair.first << ".vert" << std::endl;
		std::cout << pair.second.m_vertexShader << std::endl;
		std::cout << pair.first << ".frag" << std::endl;
		std::cout << pair.second.m_fragmentShader << std::endl;
	}

	return map;
}

void ShaderUtils::bindBlockForProgram(Shader& shader)
{
	ConstantBufferSet& globalBuffer = Shader::getShaderGlobalBuffer();
	std::vector<ShaderUniformBlockReference>& blockRefs = shader.getAllBlocks();
	auto program = shader.getShaderProgram();
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, globalBufferIdentifier.GetUbo());
	for (int i = 0;i < blockRefs.size();++i)
	{
		auto blockIndex = blockRefs[i].m_blockIndex;
		ShaderUniformBlockProperty* block;

		ConstantBufferIdentifier& globalBufferIdentifier = globalBuffer.getGlobalBufferIdentifierByBlockName(blockRefs[i].m_uniformBlockName);
		if (globalBufferIdentifier.findBlock(blockRefs[i].m_uniformBlockName, block))
		{
			glBindBufferRange(GL_UNIFORM_BUFFER, block->m_blockBindingNum, globalBufferIdentifier.GetUbo(), block->m_blockOffset, block->m_preDefineSize);
			glUniformBlockBinding(program, blockIndex, block->m_blockBindingNum);
		}
	}

}

std::string ShaderUtils::readAllText(std::string& filePath)
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

std::string ShaderUtils::extractIncludeFile(const std::string& line, const std::string& pattern)
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