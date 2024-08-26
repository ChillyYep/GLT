#pragma once
#include <GameObject.h>
#include <Component.h>

enum class LightType {
	DirectionalLight,
	SpotLight,
	PointLight
};

struct LightProperties {

	bool isEnabled;
	LightType lightType;
	glm::vec4 ambient;
	glm::vec4 color;
	glm::vec3 position;
	glm::vec3 direction;
	float spotCosCutoff;
	/// <summary>
	/// 衰减计算公式中的系数
	/// </summary>
	glm::vec4 attenuations;
};

COMPONENT_CLASS(Light, ComponentType::Light)
{
public:
	ComponentType getComponentType() override { return ComponentType::Light; }

	LightProperties getLightProperties();

	inline std::shared_ptr<Transform> getTransform() { return m_gameObjectPtr->getTransform(); }

	__GET_SET_PROPERTY__(Color, glm::vec4, m_color)
		__GET_SET_PROPERTY__(LightType, LightType, m_lightType)
		__GET_SET_BOOLEANPROPERTY__(Realtime, m_isRealtime)
		__GET_SET_PROPERTY__(Angle, float, m_angle)
		__GET_SET_PROPERTY__(ConstantAttenuation, float, m_constantAttenuation)
		__GET_SET_PROPERTY__(LinearAttenuationFactor, float, m_linearAttenuationFactor)
		__GET_SET_PROPERTY__(QuadraticAttenuationFactor, float, m_quadraticAttenuationFactor)
protected:
	LightType m_lightType;
	glm::vec4 m_color;
	bool m_isRealtime = true;

	float m_angle;
	float m_attenuationStrength;
	float m_constantAttenuation;
	float m_linearAttenuationFactor;
	float m_quadraticAttenuationFactor;
};
