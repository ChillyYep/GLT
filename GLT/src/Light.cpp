#include "Light.h"

LightProperties Light::getLightProperties()
{
	auto goPtr = static_cast<GameObject*>(m_gameObjectPtr);
	LightProperties lightProperties;
	lightProperties.isEnabled = m_enabled;
	lightProperties.lightType = m_lightType;
	lightProperties.position = goPtr->getTransform()->getPosition();
	lightProperties.direction = goPtr->getTransform()->getForward();
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