#version 450 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

layout (location = 0) out vec4 fs_Color;

uniform mat4 View,Model,Project;

void main()
{
	gl_Position = Project*View*Model*vPosition;
	fs_Color = vColor;
}