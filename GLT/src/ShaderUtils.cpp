#include "ShaderUtils.h"

ShaderUtils::ShaderUtils() {}
ShaderUtils::~ShaderUtils() {}

void ShaderUtils::loadAllShader(ScriptableRenderContext& renderContext)
{
	auto files = ShaderUtils::collectAllFiles();
	GLSLPreProcessCompiler preProcessCompiler;
	auto rawShaderInfos = preProcessCompiler.parse(files);
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