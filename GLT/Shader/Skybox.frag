#version 450 core

#include "common.attr"

in VS_Out{
	vec3 fs_Texcoord;
} fs_in;

layout (location = 0) out vec4 fColor;

layout (binding = 0) uniform samplerCube skybox;

void main()
{
	fColor = texture(skybox,fs_in.fs_Texcoord);
}