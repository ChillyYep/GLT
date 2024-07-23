#pragma once
#include <CommonDefine.h>

class ScriptableRenderContext
{
public:
	ScriptableRenderContext() {}
	~ScriptableRenderContext() {}

	void Init()
	{

	}

	GLuint CompileShader(std::string shaderName, GLuint shaderStage, const char* shaderCode, GLint shaderCodeLength)
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

	GLuint LinkProgram(std::string programName, GLuint vertexShader, GLuint fragmentShader)
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
private:

	char infoLog[512];
};