#include "Shader.h"
std::unordered_map<std::string, ShaderProgramInfo>  Shader::m_programs;
ConstantBufferSet Shader::m_globalBuffer;

Shader::Shader(std::string shaderName) {
	assert(m_programs.find(shaderName) != m_programs.end());
	m_shaderInfo = m_programs[shaderName];
}
Shader::~Shader() {
}

