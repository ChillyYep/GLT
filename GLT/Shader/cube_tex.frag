#version 450 core

#include "common.attr"
#include "lightdata.attr"

in VS_Out{
	vec4 fs_Color;
	vec2 fs_Texcoord;
	vec3 fs_worldNormal;
	vec4 fs_worldPosition;
} fs_in;

layout (location = 0) out vec4 fColor;

layout (binding = 0) uniform sampler2D mainTex;

// uniform float Shininess;
// uniform float Strength;
// uniform vec3 EyeDirection;


void main()
{
	// vec3 scatteredLight=vec3(0.0);
	// vec3 reflectedLight=vec3(0.0);
	// for(int light=0;light<MaxLights;++light)
	// {
	// 	if(!Lights[light].isEnabled)
	// 	{
	// 		continue;
	// 	}
	// 	vec3 halfVector;
	// 	vec3 lightDirection = Lights[light].position;
	// 	float attenuation=1.0;
	// 
	// 	if(Lights[light].isLocal)
	// 	{
	// 		lightDirection=lightDirection-vec3(fs_Position);
	// 	}
	// 	else
	// 	{
	// 	}
	// }

	fColor = texture(mainTex, fs_in.fs_Texcoord);
	fColor = vec4(fColor.rgb,1.0);

	// fColor=vec4(1.0,1.0,1.0,1.0);
	fColor = PhongShading_All(EyePosition.xyz,fs_in.fs_worldPosition.xyz,fs_in.fs_worldNormal,fColor,100);

}