#include "ShaderPropertyNames.h"

std::stringstream ShaderPropertyNames::s_sstream;

const char* ShaderPropertyNames::ModelMatrix = "Model";
const char* ShaderPropertyNames::ViewMatrix = "View";
const char* ShaderPropertyNames::ProjectMatrix = "Project";
const char* ShaderPropertyNames::EyePosition = "EyePosition";

const char* ShaderPropertyNames::ZBufferParams = "ZBufferParams";

const char* ShaderPropertyNames::MainLightData_Ambient = "m_mainLightData.ambient";
const char* ShaderPropertyNames::MainLightData_Color = "m_mainLightData.color";
const char* ShaderPropertyNames::MainLightData_Position = "m_mainLightData.position";
const char* ShaderPropertyNames::MainLightData_ConeDirection = "m_mainLightData.coneDirection";
const char* ShaderPropertyNames::MainLightData_EndDistance = "m_mainLightData.endDistance";
const char* ShaderPropertyNames::MainLightData_Attenuations = "m_mainLightData.attenuations";

const char* ShaderPropertyNames::AddtionalLightData = "m_additionalLightData";
const char* ShaderPropertyNames::AddtionalLightData_Ambient = "ambient";
const char* ShaderPropertyNames::AddtionalLightData_Color = "color";
const char* ShaderPropertyNames::AddtionalLightData_Position = "position";
const char* ShaderPropertyNames::AddtionalLightData_ConeDirection = "coneDirection";
const char* ShaderPropertyNames::AddtionalLightData_EndDistance = "endDistance";
const char* ShaderPropertyNames::AddtionalLightData_SpotCosCutOff = "spotCosCutoff";
const char* ShaderPropertyNames::AddtionalLightData_Attenuations = "attenuations";

const char* ShaderPropertyNames::MVP = "MVP";
const char* ShaderPropertyNames::VP = "VP";

const char* ShaderPropertyNames::MainTex = "mainTex";

const char* ShaderPropertyNames::Ambient = "Ambient";

const char* ShaderPropertyNames::Lights = "Lights";