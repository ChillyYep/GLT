#version 450 core

#include "common.attr"

layout (location = 0) in vec4 vPosition;

out VS_Out{
	float fs_z;
} vs_out;

void main()
{
	mat4 MVP = Project*View*Model;
	gl_Position = MVP*vPosition;
	vs_out.fs_z = clamp((gl_Position.z/gl_Position.w+1.0)*0.5,0,1);
}