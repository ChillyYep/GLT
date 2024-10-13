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
uniform sampler2DShadow shadowMapTex;

void main()
{

	fColor = texture(mainTex, fs_in.fs_Texcoord);
	fColor = vec4(fColor.rgb,1.0);

//	vec3 texPos=fs_in.fs_shadowCoord.xyz;
//	texPos/=fs_in.fs_shadowCoord.w;
//	texPos=(texPos+vec3(1,1,1))*0.5;

//	float depth=texture(shadowMapTex,texPos.xy);
	float shadowStrength = textureProj(shadowMapTex,fs_in.fs_shadowCoord);
//	fColor = PhongShading_All(EyePosition.xyz,fs_in.fs_worldPosition.xyz,fs_in.fs_worldNormal,fColor,10,shadowStrength);
	fColor = shadowStrength.rrrr;
}