#version 450 core

layout (location = 0) in vec4 fs_Color;

layout (location = 0) out vec4 fColor;

void main()
{
	// fColor = vec4(0.5,0.4,0.8,1.0);
	fColor = fs_Color;
}