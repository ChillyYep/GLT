#pragma once
#include <CommonDefine.h>
#include <ConstantBuffers.h>
#include <filesystem>
namespace fs = std::filesystem;

struct ShaderProgramInfo
{
	std::string m_programName;
	GLuint m_program;
	std::vector<ShaderUniformProperty> m_uniformProperties;
	std::vector<ShaderUniformBlockReference> m_referenceBlocks;
};

class Shader
{
public:
	Shader(std::string shaderName);
	~Shader();

	inline std::string GetShaderName() { return m_shaderInfo.m_programName; }

	inline GLuint GetShaderProgram() { return m_shaderInfo.m_program; }

	inline std::vector<ShaderUniformBlockReference>& GetAllBlocks() { return m_shaderInfo.m_referenceBlocks; }

	inline void SetMatrix(const char* propertyName, glm::mat4x4 matrix)
	{
		GLuint shaderProgram = m_shaderInfo.m_program;
		if (shaderProgram > 0)
		{
			GLuint matrixLoc = glGetUniformLocation(shaderProgram, propertyName);
			if (matrixLoc >= 0)
			{
				glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(matrix));
			}
		}
	}

	inline void SetVector(const char* propertyName, glm::vec4 vec4)
	{
		GLuint shaderProgram = m_shaderInfo.m_program;
		if (shaderProgram > 0)
		{
			GLuint vecLoc = glGetUniformLocation(shaderProgram, propertyName);
			glProgramUniform4f(shaderProgram, vecLoc, vec4.x, vec4.y, vec4.z, vec4.w);
		}
	}

	static inline void Init(std::unordered_map<std::string, ShaderProgramInfo>& allPrograms, std::vector<ConstantBufferIdentifier>& constantBuffers)
	{
		m_allPrograms = allPrograms;
		for (int i = 0;i < constantBuffers.size();++i)
		{
			m_globalBuffer.Create(constantBuffers[i]);
		}
	}

	static inline void SetGlobalMatrix(const char* propertyName, glm::mat4x4 matrix)
	{
		_SetGlobalValueInternal(propertyName, matrix);
	}

	static inline void SetGlobalFloat(const char* propertyName, float value)
	{
		_SetGlobalValueInternal(propertyName, value);
	}

	static inline void SetGlobalVector(const char* propertyName, glm::vec4 vec4)
	{
		_SetGlobalValueInternal(propertyName, vec4);
	}

	static inline void Upload(ConstantBufferType constantBufferType)
	{
		m_globalBuffer.Upload(constantBufferType);
	}
	static inline ConstantBufferSet& GetShaderGlobalBuffer() { return m_globalBuffer; }
	// Todo:需要有卸载的接口，用于shader切换，或者将所有shader一次性载入且不再卸载
private:
	template<typename T>
	static inline void _SetGlobalValueInternal(const char* propertyName, T value)
	{
		ShaderUniformBlockProperty block;
		BlockUniform blockUniform;
		ConstantBufferType constantBufferType;
		m_globalBuffer.FindProperty(propertyName, block, blockUniform, constantBufferType);
		if (block != ShaderUniformBlockProperty::Null() && blockUniform != BlockUniform::Null())
		{
			GLuint offset, length;
			m_globalBuffer.GetPropertyRange(block, blockUniform, &offset, &length);
			if (sizeof(T) <= length)
			{
				m_globalBuffer.SetData(constantBufferType, offset, length, reinterpret_cast<void*>(&value));
			}
		}
	}
	ShaderProgramInfo m_shaderInfo;

	static std::unordered_map<std::string, ShaderProgramInfo> m_allPrograms;

	static ConstantBufferSet m_globalBuffer;
};