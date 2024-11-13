#version 450 core
#include "common.attr"
// in
layout (binding = 0) uniform sampler2D depthBuffer;
layout (binding = 1) uniform sampler2D mainTex;

uniform vec4 samples[64];

// out
layout (location = 0) out vec4 fColor;

in VS_Out{
	vec2 fs_Texcoord;
} fs_in;

vec3 GetCameraPositionFromDepthValue(vec2 texcoord,float depth)
{
	float camPosZ=ProjectParams.y+(ProjectParams.z-ProjectParams.y)*depth;

	float height=2*camPosZ/Project[1][1];
	float width=ScreenParams.x/ScreenParams.y*height;
	float camPosX=width*texcoord.x-width/2;
	float camPosY=height*texcoord.y-height/2;
	return vec3(camPosX,camPosY,camPosZ);
}

void main()
{
	float curDepth = texture(depthBuffer,fs_in.fs_Texcoord).r;

	vec3 camPos = GetCameraPositionFromDepthValue(fs_in.fs_Texcoord,curDepth);
	
	float occlussion = 0.0f;
	for(int i=0;i<64;++i)
	{
		vec3 samplePos=camPos+samples[i].xyz*0.001;
		vec4 projectPos=Project*vec4(samplePos,1.0);
		projectPos/=projectPos.w;
		float sampleDepth = texture(depthBuffer,projectPos.xy).r;
		occlussion+=(projectPos.z+1)*0.5 > sampleDepth?1.0:0.0;
	}
	occlussion = max(occlussion - 32,0) / 32;

	//fColor = texture(mainTex, fs_in.fs_Texcoord);
	fColor = (1-occlussion).xxxx;
}