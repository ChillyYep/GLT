#include "Shader.h"
std::unordered_map<std::string, ShaderProgramInfo>  Shader::m_allPrograms;
ConstantBufferSet Shader::m_globalBuffer;

Shader::Shader(std::string shaderName) {
	assert(m_allPrograms.find(shaderName) != m_allPrograms.end());
	m_shaderInfo = m_allPrograms[shaderName];
}
Shader::~Shader() {
}

