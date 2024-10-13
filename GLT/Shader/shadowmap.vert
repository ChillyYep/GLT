#version 450 core

#include "common.attr"

layout (location = 0) in vec4 vPosition;

void main()
{
	mat4 MV = View*Model;
	mat4 MVP = Project*MV;
	gl_Position = MVP*vPosition;
}