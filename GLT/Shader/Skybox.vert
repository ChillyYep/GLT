#version 450 core

#include "common.attr"

layout (location = 0) in vec4 vPosition;

out VS_Out{
	vec3 fs_Texcoord;
} vs_out;


void main()
{
	gl_Position = Project * View * Model * vPosition;
	vs_out.fs_Texcoord = vec3(vPosition.x,vPosition.y,vPosition.z);
}