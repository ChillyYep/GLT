#pragma once
#include <GLMath.h>
#include <vector>

enum class LightType {
	DirectionalLight,
	SpotLight,
	PointLight
};

struct CameraData {
	glm::vec3 m_worldPos;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
	glm::vec4 m_viewport;
};
struct LightProperties {

	bool isEnabled;
	LightType lightType;
	glm::vec4 ambient;
	glm::vec4 color;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 direction;
	float spotCosCutoff;
	/// <summary>
	/// 衰减计算公式中的系数
	/// </summary>
	glm::vec4 attenuations;
};
struct RenderData
{
	int m_curRenderingCameraIndex;
	std::vector<CameraData> m_cameraDatas;
	std::vector<LightProperties> m_lightDatas;
};