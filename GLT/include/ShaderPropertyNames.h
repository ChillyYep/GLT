#pragma once
/// <summary>
/// Shader属性名集合
/// </summary>
class ShaderPropertyNames {
public:
	static const char* ModelMatrix;
	static const char* ViewMatrix;
	static const char* ProjectMatrix;
	static const char* EyePosition;

	static const char* MainLightData_Ambient;
	static const char* MainLightData_Color;
	static const char* MainLightData_Position;
	static const char* MainLightData_ConeDirection;
	static const char* MainLightData_EndDistance;
	static const char* MainLightData_Attenuations;

	static const char* AddtionalLightData_Format;
	static const char* AddtionalLightData_Ambient_Format;
	static const char* AddtionalLightData_Color_Format;
	static const char* AddtionalLightData_Position_Format;
	static const char* AddtionalLightData_ConeDirection_Format;
	static const char* AddtionalLightData_EndDistance_Format;
	static const char* AddtionalLightData_Attenuations_Format;
	static const char* AddtionalLightData_SpotCosCutOff_Format;


	static const char* MVP;
	static const char* VP;

	static const char* MainTex;
	static const char* Ambient;
	static const char* Lights;
};