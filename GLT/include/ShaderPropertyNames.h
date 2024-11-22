#pragma once
#include <sstream>
/// <summary>
/// Shader属性名集合
/// </summary>
class ShaderPropertyNames {
public:
	static const char* ModelMatrix;
	static const char* ViewMatrix;
	static const char* ProjectMatrix;
	static const char* InverseViewMatrix;
	static const char* InverseProjectMatrix;
	static const char* ProjectParams;
	static const char* ScreenParams;
	static const char* EyePosition;

	static const char* ZBufferParams;

	static const char* MainLightData_Ambient;
	static const char* MainLightData_Color;
	static const char* MainLightData_Position;
	static const char* MainLightData_ConeDirection;
	static const char* MainLightData_EndDistance;
	static const char* MainLightData_Attenuations;

	static const char* AddtionalLightData;
	static const char* AddtionalLightData_Ambient;
	static const char* AddtionalLightData_Color;
	static const char* AddtionalLightData_Position;
	static const char* AddtionalLightData_ConeDirection;
	static const char* AddtionalLightData_EndDistance;
	static const char* AddtionalLightData_Attenuations;
	static const char* AddtionalLightData_SpotCosCutOff;


	static const char* ShadowMatrixVP;
	static const char* ShadowBias;
	static const char* MVP;
	static const char* VP;

	static const char* SkyBox;
	static const char* MainTex;
	static const char* DepthBuffer;
	static const char* ShadowMapTex;
	static const char* Ambient;
	static const char* Lights;

	static const char* CommonAlpha;

	inline static std::string getShaderArrayPropertyName(const char* arrayPropertyName, const char* memberPropertyName, int index)
	{
		s_sstream.str("");
		s_sstream << arrayPropertyName << "[" << index << "]." << memberPropertyName;
		std::string str = s_sstream.str();
		return str;
	}

	static std::stringstream s_sstream;
};