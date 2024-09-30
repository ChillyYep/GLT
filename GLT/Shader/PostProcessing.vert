#version 450 core

layout (location = 0) in vec4 vPosition;

out VS_Out{
	vec2 fs_Texcoord;
} vs_out;

void main()
{
	gl_Position = float4(vPosition.xy,0.0,1.0);
	vs_out.fs_Texcoord = vPosition.xy;
}