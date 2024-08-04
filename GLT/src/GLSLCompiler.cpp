#include "GLSLCompiler.h"

void GLSLCompiler::compileShaderProgram(std::unordered_map<std::string, RawShaderInfo>& rawShaderInfo, CompiledResult& compiledResult)
{
	for (const auto& shaderInfo : rawShaderInfo)
	{
		auto shaderProgramName = shaderInfo.first;

		auto vertexShaderCode = shaderInfo.second.m_vertexShader.c_str();
		GLint vertexShaderCodeLength = (GLint)shaderInfo.second.m_vertexShader.size();

		auto fragmentShaderCode = shaderInfo.second.m_fragmentShader.c_str();
		GLint fragmentShaderCodeLength = (GLint)shaderInfo.second.m_fragmentShader.size();

		auto vertexShader = compileShader(shaderProgramName, GL_VERTEX_SHADER, vertexShaderCode, vertexShaderCodeLength);
		auto fragmentShader = compileShader(shaderProgramName, GL_FRAGMENT_SHADER, fragmentShaderCode, fragmentShaderCodeLength);

		auto program = linkProgram(shaderProgramName, vertexShader, fragmentShader);
		ShaderProgramInfo shaderProgramInfo;
		shaderProgramInfo.m_programName = shaderProgramName;
		shaderProgramInfo.m_program = program;

		afterEveryProgramCompiled(program, shaderProgramInfo, compiledResult.constantBuffers);

		compiledResult.outputShaderPrograms[shaderProgramName] = shaderProgramInfo;
	}
	afterAllProgramCompiled(compiledResult.outputShaderPrograms, compiledResult.constantBuffers);
}

GLuint GLSLCompiler::compileShader(std::string shaderName, GLuint shaderStage, const char* shaderCode, GLint shaderCodeLength)
{
	int success;
	GLuint shader = glCreateShader(shaderStage);
	glShaderSource(shader, 1, &shaderCode, &shaderCodeLength);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		memset(infoLog, 0, sizeof(infoLog));
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cout << shaderName << " ";
		std::cout << (shaderStage == GL_VERTEX_SHADER ? "vertex" : "fragment");
		std::cout << " SHADER::COMPLICATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

GLuint GLSLCompiler::linkProgram(std::string programName, GLuint vertexShader, GLuint fragmentShader)
{
	int success;
	if (vertexShader > 0 && fragmentShader > 0)
	{
		auto program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			memset(infoLog, 0, sizeof(infoLog));
			glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
			std::cout << programName << " SHADER::LINK_FAILED\n" << infoLog << std::endl;
		}
		return program;
	}
	return 0;
}

void GLSLCompiler::afterEveryProgramCompiled(GLuint program, ShaderProgramInfo& shaderProgramInfo, std::vector<ConstantBufferIdentifier>& constantBuffers)
{
	GLint maxPropertyNameLength, numProperties, maxBlockNameLength, numBlocks;

	getProgramInfo(program, &maxPropertyNameLength, &numProperties, &maxBlockNameLength, &numBlocks);

	auto uniformNames = extractUniformNames(program, numProperties, maxPropertyNameLength);
	size_t uniformsCount = uniformNames.size();

	std::vector<GLuint> uniformIndcies = std::vector<GLuint>(uniformsCount);
	std::vector<GLint> uniformSizes = std::vector<GLint>(uniformsCount);
	std::vector<GLint> uniformOffsets = std::vector<GLint>(uniformsCount);
	std::vector<GLint> uniformTypes = std::vector<GLint>(uniformsCount);
	extractUniformIndices(program, uniformNames, uniformIndcies, uniformSizes, uniformOffsets, uniformTypes);

	extractBlocks(program, numBlocks, maxBlockNameLength, uniformNames, uniformIndcies, uniformSizes, uniformOffsets, uniformTypes, constantBuffers, shaderProgramInfo.m_referenceBlocks);

	extractNoBlockUniforms(program, numProperties, maxPropertyNameLength, uniformNames, uniformSizes, uniformOffsets, uniformTypes, shaderProgramInfo.m_uniformProperties);
}

void GLSLCompiler::afterAllProgramCompiled(const std::unordered_map<std::string, ShaderProgramInfo>& outputShaderPrograms, std::vector<ConstantBufferIdentifier>& constantBuffers)
{
	int count = 0;
	for (int i = 0;i < constantBuffers.size();++i)
	{
		constantBuffers[i].ReCalculateBufferSizeAndBlockOffset();
		auto& blocks = constantBuffers[i].GetBlocks();
		for (int j = 0;j < blocks.size();++j)
		{
			blocks[j].m_blockBindingNum = count++;
		}
	}

	//for (const auto& shaderProgramInfoPair : outputShaderPrograms)
	//{
	//	auto shaderName = shaderProgramInfoPair.first;
	//	auto shaderProgramInfo = shaderProgramInfoPair.second;
	//	for (const auto& blockRef : shaderProgramInfo.m_referenceBlocks)
	//	{
	//		int targetConstantBufferIndex = -1;
	//		auto blockPtr = FindBlock(constantBuffers, blockRef.m_uniformBlockName, &targetConstantBufferIndex);
	//		if (blockPtr != nullptr)
	//		{
	//			// 建立buffer和绑定点的联系，认为是可以固定的
	//			glBindBufferRange(GL_UNIFORM_BUFFER, blockPtr->m_blockBindingNum, constantBuffers[targetConstantBufferIndex].GetUbo(), blockPtr->m_blockOffset, blockPtr->m_preDefineSize);
	//			//glUniformBlockBinding(shaderProgramInfo.m_program, blockRef.m_blockIndex, block->m_blockBindingNum);
	//		}
	//	}
	//}
}

void GLSLCompiler::extractBlocks(GLuint program, GLint numBlocks, GLint maxBlockNameLength, const std::vector<std::string>& uniformNames, const std::vector<GLuint>& uniformIndcies,
	const std::vector<GLint>& uniformSizes, const std::vector<GLint>& uniformOffsets, const std::vector<GLint>& uniformTypes,
	std::vector<ConstantBufferIdentifier>& constantBuffers, std::vector<ShaderUniformBlockReference>& blockReferences)
{
	std::vector<GLchar> propertyNameBuff = std::vector<GLchar>(maxBlockNameLength);
	GLsizei blockNameLength;
	for (GLint i = 0;i < numBlocks;++i)
	{
		ShaderUniformBlockProperty blockProperty;
		memset(propertyNameBuff.data(), '\0', propertyNameBuff.size());
		glGetActiveUniformBlockName(program, i, maxBlockNameLength, &blockNameLength, propertyNameBuff.data());
		// 获取block索引
		GLint blockIndex = glGetUniformBlockIndex(program, propertyNameBuff.data());
		blockProperty.m_name = std::string(propertyNameBuff.data());
		blockProperty.m_blockBindingNum = -1;
		blockProperty.m_constantBufferType = getConstantType(blockProperty.m_name);
		blockProperty.m_blockOffset = 0;
		if (blockIndex >= 0)
		{
			// 获取block内部uniform变量数量
			GLint numBlockActivedUniforms;
			glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numBlockActivedUniforms);

			std::vector<GLint> blockUniformsIndices = std::vector<GLint>(numBlockActivedUniforms);

			glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, blockUniformsIndices.data());

			for (GLint j = 0;j < numBlockActivedUniforms;++j)
			{
				auto index = getUniformIndex(blockUniformsIndices[j], uniformNames, uniformIndcies);
				BlockUniform blockUniform;
				blockUniform.m_uniformName = uniformNames[index];
				blockUniform.m_type = uniformTypes[index];
				blockUniform.m_offset = uniformOffsets[index];
				blockUniform.m_size = uniformSizes[index] * typeSize(uniformTypes[index]);
				blockProperty.m_blockUniforms.push_back(blockUniform);
			}

			ShaderUniformBlockReference blockReference;
			blockReference.m_uniformBlockName = blockProperty.m_name;
			blockReference.m_blockIndex = blockIndex;
			blockReferences.push_back(blockReference);
			blockProperty.m_preDefineSize = getPreDefineSize(blockProperty.m_blockUniforms);

			// 先查询block是否已经存在，如果不存在，尝试添加该block，如果已经存在，比较二者是否相同，不同表示异常
			auto targetBlockPtr = findBlock(constantBuffers, blockProperty.m_name);
			if (targetBlockPtr == nullptr)
			{
				// 获取constantbuffer，如果不存在新建一个，并将Block存入
				auto targetConstantBuffer = getConstantBuffer(constantBuffers, blockProperty.m_constantBufferType);
				if (targetConstantBuffer == nullptr)
				{
					constantBuffers.push_back(ConstantBufferIdentifier());
					targetConstantBuffer = &constantBuffers[constantBuffers.size() - 1];
					targetConstantBuffer->SetConstantBufferType(blockProperty.m_constantBufferType);
				}
				targetConstantBuffer->AddBlock(blockProperty);
			}
			else
			{
				assert(*targetBlockPtr != blockProperty);
			}
		}
	}
}

void GLSLCompiler::extractNoBlockUniforms(GLuint program, GLint numProperties, GLint maxPropertyNameLength, const std::vector<std::string>& uniformNames, const std::vector<GLint>& uniformSizes,
	const std::vector<GLint>& uniformOffsets, const std::vector<GLint>& uniformTypes, std::vector<ShaderUniformProperty>& uniformProperties)
{
	for (GLint i = 0;i < numProperties;++i)
	{
		GLint location = glGetUniformLocation(program, uniformNames[i].data());
		// 程序内部uniform变量
		if (location >= 0)
		{
			ShaderUniformProperty property;
			property.m_name = std::string(uniformNames[i]);
			property.m_location = location;
			property.m_type = uniformTypes[i];
			property.m_offset = uniformOffsets[i];
			property.m_size = uniformSizes[i] * typeSize(uniformTypes[i]);
			uniformProperties.push_back(property);
		}
	}
}

void GLSLCompiler::extractUniformIndices(GLuint program, std::vector<std::string>& uniformNames, std::vector<GLuint>& uniformIndcies, std::vector<GLint>& uniformSizes,
	std::vector<GLint>& uniformOffsets, std::vector<GLint>& uniformTypes)
{
	GLsizei numProperties = (GLsizei)uniformNames.size();
	std::vector<const GLchar*> uniformNamePtrs = std::vector<const GLchar*>(numProperties);
	for (int i = 0;i < numProperties;++i)
	{
		uniformNamePtrs[i] = uniformNames[i].c_str();
	}
	glGetUniformIndices(program, numProperties, uniformNamePtrs.data(), uniformIndcies.data());
	glGetActiveUniformsiv(program, numProperties, uniformIndcies.data(), GL_UNIFORM_SIZE, uniformSizes.data());
	glGetActiveUniformsiv(program, numProperties, uniformIndcies.data(), GL_UNIFORM_OFFSET, uniformOffsets.data());
	glGetActiveUniformsiv(program, numProperties, uniformIndcies.data(), GL_UNIFORM_TYPE, uniformTypes.data());
}

std::vector<std::string> GLSLCompiler::extractUniformNames(GLuint program, GLint numProperties, GLint maxPropertyNameLength)
{
	std::vector<GLchar> propertyNameBuff = std::vector<GLchar>(maxPropertyNameLength);
	GLsizei propertyNameLength;
	std::vector<std::string> uniformNames = std::vector<std::string>(numProperties);
	for (GLint i = 0;i < numProperties;++i)
	{
		memset(propertyNameBuff.data(), '\0', propertyNameBuff.size());
		glGetActiveUniformName(program, i, maxPropertyNameLength, &propertyNameLength, propertyNameBuff.data());
		std::string temp = std::string(propertyNameLength, '\0');
		memcpy(temp.data(), propertyNameBuff.data(), propertyNameLength);
		uniformNames[i] = temp;
	}
	return uniformNames;
}

void GLSLCompiler::getProgramInfo(GLuint program, GLint* maxPropertyNameLength, GLint* numProperties, GLint* maxBlockNameLength, GLint* numBlocks)
{
	glGetProgramInterfaceiv(program, GL_UNIFORM, GL_MAX_NAME_LENGTH, maxPropertyNameLength);
	glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES, numProperties);
	glGetProgramInterfaceiv(program, GL_UNIFORM_BLOCK, GL_MAX_NAME_LENGTH, maxBlockNameLength);
	glGetProgramInterfaceiv(program, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, numBlocks);
}

size_t GLSLCompiler::typeSize(GLenum type)
{
	size_t size = 0;
#define CASE(Enum,Count,Type) \
case Enum : size = Count * sizeof(Type); break;
	switch (type)
	{
		CASE(GL_FLOAT, 1, GLfloat);
		CASE(GL_FLOAT_VEC2, 2, GLfloat);
		CASE(GL_FLOAT_VEC3, 3, GLfloat);
		CASE(GL_FLOAT_VEC4, 4, GLfloat);
		CASE(GL_INT, 1, GLint);
		CASE(GL_INT_VEC2, 2, GLint);
		CASE(GL_INT_VEC3, 3, GLint);
		CASE(GL_INT_VEC4, 4, GLint);
		CASE(GL_UNSIGNED_INT, 1, GLuint);
		CASE(GL_UNSIGNED_INT_VEC2, 2, GLuint);
		CASE(GL_UNSIGNED_INT_VEC3, 3, GLuint);
		CASE(GL_UNSIGNED_INT_VEC4, 4, GLuint);

		CASE(GL_BOOL, 1, GLboolean);
		CASE(GL_BOOL_VEC2, 2, GLboolean);
		CASE(GL_BOOL_VEC3, 3, GLboolean);
		CASE(GL_BOOL_VEC4, 4, GLboolean);

		CASE(GL_FLOAT_MAT2, 4, GLfloat);
		CASE(GL_FLOAT_MAT2x3, 6, GLfloat);
		CASE(GL_FLOAT_MAT2x4, 8, GLfloat);
		CASE(GL_FLOAT_MAT3x2, 6, GLfloat);
		CASE(GL_FLOAT_MAT3, 9, GLfloat);
		CASE(GL_FLOAT_MAT3x4, 12, GLfloat);
		CASE(GL_FLOAT_MAT4x2, 8, GLfloat);
		CASE(GL_FLOAT_MAT4x3, 12, GLfloat);
		CASE(GL_FLOAT_MAT4, 16, GLfloat);
#undef CASE
	default:
		break;
	}
	return size;
}

bool GLSLCompiler::isTypeSupportBlock(GLenum type)
{
	return typeSize(type) > 0;
}

GLuint GLSLCompiler::getPreDefineSize(std::vector<BlockUniform>& blockUniforms)
{
	if (blockUniforms.size() == 0)
	{
		return 0;
	}
#define ALIGN(size,alignment) ((size - 1) / alignment + 1) * alignment
	GLint maxOffset = -1;
	GLint maxOffsetSize = 0;
	for (int i = 0;i < blockUniforms.size();++i)
	{
		if (blockUniforms[i].m_offset > maxOffset)
		{
			maxOffset = blockUniforms[i].m_offset;
			maxOffsetSize = (GLint)blockUniforms[i].m_size;
		}
	}
	return ALIGN(maxOffset + maxOffsetSize, 16);
#undef ALIGN
}

ShaderUniformBlockProperty* GLSLCompiler::findBlock(std::vector<ConstantBufferIdentifier>& constantBuffers, std::string blockName, int* constantBufferIndex)
{
	ShaderUniformBlockProperty* blockPtr;
	for (int i = 0;i < constantBuffers.size();++i)
	{
		if (constantBuffers[i].FindBlock(blockName, blockPtr))
		{
			if (constantBufferIndex != nullptr)
			{
				*constantBufferIndex = i;
			}
			return blockPtr;
		}
	}
	return nullptr;
}

int GLSLCompiler::getUniformIndex(GLuint index, const std::vector<std::string>& uniformNames, const std::vector<GLuint>& uniformIndcies)
{
	for (int i = 0;i < uniformIndcies.size();++i)
	{
		if (index == uniformIndcies[i])
		{
			return i;
		}
	}
	return -1;
}

ConstantBufferType GLSLCompiler::getConstantType(std::string blockName)
{
	if (StringUtility::EndsWith(blockName, "PerFrame"))
	{
		return ConstantBufferType::PerFrame;
	}
	if (StringUtility::EndsWith(blockName, "PerCamera"))
	{
		return ConstantBufferType::PerCamera;
	}
	if (StringUtility::EndsWith(blockName, "PerPass"))
	{
		return ConstantBufferType::PerPass;
	}
	return ConstantBufferType::None;
}

ConstantBufferIdentifier* GLSLCompiler::getConstantBuffer(std::vector<ConstantBufferIdentifier>& constantBuffers, ConstantBufferType constantBufferType)
{
	for (int i = 0;i < constantBuffers.size();++i)
	{
		if (constantBuffers[i].GetConstantBufferType() == constantBufferType)
		{
			return &constantBuffers[i];
		}
	}
	return nullptr;
}
