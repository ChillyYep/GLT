#version 450 core

#include "common.attr"

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vTexcoord;
layout (location = 3) in vec4 vNormal;

out VS_Out{
	vec4 fs_Color;
	vec2 fs_Texcoord;
	vec3 fs_worldNormal;
	vec4 fs_worldPosition;
} vs_out;

void main()
{
	mat4 MV = View*Model;
	mat4 MVP = Project*MV;
	gl_Position = MVP*vPosition;
	vs_out.fs_Color = vColor;
	vs_out.fs_worldPosition = Model*vPosition;
	vs_out.fs_Texcoord = vTexcoord;
	vs_out.fs_worldNormal = normalize((Model*vNormal-Model*vec4(0,0,0,1)).xyz);
}