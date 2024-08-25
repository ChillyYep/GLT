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

	virtual LightProperties getLightProperties()
	{
		LightProperties lightProperties;
		lightProperties.isEnabled = m_enabled;
		lightProperties.lightType = m_lightType;
		lightProperties.position = getTransform()->getPosition();
		lightProperties.direction = getTransform()->Forward();
		lightProperties.color = m_color;
		lightProperties.spotCosCutoff = 0.0f;
		lightProperties.ambient = m_color * 0.1f;
		if (m_lightType == LightType::DirectionalLight)
		{
			lightProperties.attenuations = glm::zero<glm::vec4>();
		}
		else if (m_lightType == LightType::SpotLight)
		{
			lightProperties.spotCosCutoff = glm::radians(m_angle / 2);
			lightProperties.attenuations = glm::vec4(m_attenuationStrength, m_constantAttenuation, m_linearAttenuationFactor, m_quadraticAttenuationFactor);
		}
		else if (m_lightType == LightType::PointLight)
		{
			lightProperties.attenuations = glm::vec4(m_attenuationStrength, m_constantAttenuation, m_linearAttenuationFactor, m_quadraticAttenuationFactor);
		}
		return lightProperties;
	}

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
