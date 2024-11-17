#version 450 core
#include "common.attr"
// in
layout (location = 0,binding = 0) uniform sampler2D mainTex;
layout (location = 1,binding = 1) uniform sampler2D depthBuffer;

uniform vec4 samples[64];

// out
layout (location = 0) out vec4 fColor;

in VS_Out{
	vec2 fs_Texcoord;
} fs_in;
float GetLinearDepth(float rawDepth,float n,float f)
{
	return ( n*f / (f-rawDepth*(f-n))-n)/(f-n);
}

vec3 GetCameraPositionFromDepthValue(vec2 texcoord,float depth)
{
	// 采样出来的深度从近到远，数值从1到0，ProjectParams.y和ProjectParams.z分别表示近平面和远平面
	float camPosZ = ProjectParams.y + (ProjectParams.z - ProjectParams.y) * depth;
	float height = 2*camPosZ/Project[1][1];
	float width = ScreenParams.x/ScreenParams.y*height;
	float camPosX = width*texcoord.x-width/2;
	float camPosY = height*texcoord.y-height/2;
	// 相机的Z轴负方向为视角方向，因此需要取反
	return vec3(camPosX,camPosY,-camPosZ);
}

void main()
{
	float curDepth = texture(depthBuffer,fs_in.fs_Texcoord).r;
	float linearDepth = GetLinearDepth(curDepth,ProjectParams.y,ProjectParams.z);
	vec3 camPos = GetCameraPositionFromDepthValue(fs_in.fs_Texcoord,linearDepth);

	/*
	vec4 projectPos = Project * vec4(camPos,1.0);
	projectPos /= projectPos.w;
	projectPos.xyz = (vec3(1.0) + projectPos.xyz) * 0.5;
	if(fs_in.fs_Texcoord.x < 0.5)
	{
		fColor = projectPos.zzzz;
	}
	else{
		fColor = curDepth.xxxx;
	}
	*/
	float occlussion = 0.0f;
	for(int i = 0;i < 64;++i)
	{
		vec3 samplePos = camPos + samples[i].xyz * 0.05;
		samplePos.z+=0.0001;
		vec4 projectPos = Project * vec4(samplePos,1.0);
		projectPos /= projectPos.w;
		projectPos.xyz = (vec3(1.0) + projectPos.xyz)*0.5;
		float sampleDepth = texture(depthBuffer,projectPos.xy).r;
		occlussion += step(projectPos.z,sampleDepth);
		//occlussion += sampleDepth;
		//occlussion += projectPos.z;
	}
	occlussion = max(occlussion - 32,0) / 32;
	//occlussion = occlussion / 64;

	fColor = texture(mainTex, fs_in.fs_Texcoord)*(1-occlussion);
	//fColor = occlussion.xxxx;
}