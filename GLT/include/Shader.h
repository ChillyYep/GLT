#pragma once
#include <CommonDefine.h>
#include <ConstantBuffers.h>
#include <filesystem>
#include <GLMath.h>
namespace fs = std::filesystem;

class Shader
{
public:
	Shader(std::string shaderName);
	~Shader();

	inline std::string getShaderName() { return m_shaderInfo.m_programName; }

	inline GLuint getShaderProgram() { return m_shaderInfo.m_program; }

	inline std::vector<ShaderUniformProperty>& getShaderUniforms() { return m_shaderInfo.m_uniformProperties; }

	inline std::vector<ShaderUniformBlockReference>& getReferencedBlocks() { return m_shaderInfo.m_referenceBlocks; }

	static inline void init(std::unordered_map<std::string, ShaderProgramInfo>& allPrograms, std::vector<ConstantBufferIdentifier>& constantBuffers)
	{
		m_allPrograms = allPrograms;
		for (int i = 0;i < constantBuffers.size();++i)
		{
			m_globalBuffer.create(constantBuffers[i]);
		}
	}

	static inline void setGlobalMatrix(const char* propertyName, glm::mat4x4 matrix)
	{
		_setGlobalValueInternal(propertyName, matrix);
	}

	static inline void setGlobalFloat(const char* propertyName, float value)
	{
		_setGlobalValueInternal(propertyName, value);
	}

	static inline void setGlobalInt(const char* propertyName, int value)
	{
		_setGlobalValueInternal(propertyName, value);
	}

	static inline void setGlobalBool(const char* propertyName, bool value)
	{
		_setGlobalValueInternal(propertyName, value);
	}
	
	static inline void setGlobalVector(const char* propertyName, glm::vec4 vec4)
	{
		_setGlobalValueInternal(propertyName, vec4);
	}

	static inline ConstantBufferSet& getShaderConstantBufferSet() { return m_globalBuffer; }
	// Todo:需要有卸载的接口，用于shader切换，或者将所有shader一次性载入且不再卸载
private:
	template<typename T>
	static inline void _setGlobalValueInternal(const char* propertyName, T value)
	{
		ShaderUniformBlockProperty block;
		BlockUniform blockUniform;
		ConstantBufferType constantBufferType;
		m_globalBuffer.findProperty(propertyName, block, blockUniform, constantBufferType);
		if (block != ShaderUniformBlockProperty::null() && blockUniform != BlockUniform::null())
		{
			GLuint offset, length;
			m_globalBuffer.getPropertyRange(block, blockUniform, &offset, &length);
			if (sizeof(T) <= length)
			{
				m_globalBuffer.setData(constantBufferType, offset, length, reinterpret_cast<void*>(&value));
			}
		}
	}
	ShaderProgramInfo m_shaderInfo;

	static std::unordered_map<std::string, ShaderProgramInfo> m_allPrograms;

	static ConstantBufferSet m_globalBuffer;
};