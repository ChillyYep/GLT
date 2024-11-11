#version 450 core
// in
layout (binding = 0) uniform sampler2D mainTex;

// out
layout (location = 0) out vec4 fColor;

in VS_Out{
	vec2 fs_Texcoord;
} fs_in;

void main()
{
	fColor = texture(mainTex, fs_in.fs_Texcoord);
	float gray = fColor.r*0.3+fColor.g*0.59+fColor.b*0.11;
	fColor = vec4(gray,gray,gray,1.0);
}