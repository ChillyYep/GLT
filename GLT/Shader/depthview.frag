#version 450 core

in VS_Out{
	float fs_z;
} fs_in;

layout (location = 0) out vec4 fColor;

void main()
{
	fColor = vec4(fs_in.fs_z,0.0,0.0,1.0);

}