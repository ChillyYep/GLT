#version 450 core

#include "common.attr"
#include "lightdata.attr"

in VS_Out{
	vec4 fs_Color;
	vec2 fs_Texcoord;
	vec3 fs_worldNormal;
	vec4 fs_worldPosition;
	vec4 fs_shadowCoord;
} fs_in;

layout (location = 0) out vec4 fColor;

layout (binding = 0) uniform sampler2D mainTex;
layout (binding = 1) uniform sampler2D shadowMapTex;

void main()
{
	fColor = texture(mainTex, fs_in.fs_Texcoord);
	float shadowStrength = PCFShadowCalculation(fs_in.fs_shadowCoord,shadowMapTex,0.008);
	fColor = PhongShading_All(EyePosition.xyz,fs_in.fs_worldPosition.xyz,fs_in.fs_worldNormal,fColor,10)*(1-shadowStrength);
}