#pragma once
#include <Transform.h>
#include<Component.h>

class DirectionalLight;
class SpotLight;
class PointLight;

struct LightProperties {

	bool isEnabled;
	bool isLocal;
	bool isSpot;
	glm::vec4 ambient;
	glm::vec4 color;
	glm::vec3 position;
	glm::vec3 coneDirection;
	float spotCosCutoff;
	/// <summary>
	/// 衰减计算公式中的系数
	/// </summary>
	glm::vec4 attenuations;
};

COMPONENT_CLASS(Light, ComponentType::ComponentType_Light)
{
public:
	ComponentType GetComponentType() override { return ComponentType::ComponentType_Light; }

	virtual LightProperties GetLightProperties() = 0;
	inline static int GetMaxLightCount()
	{
		return 16;
	}

	inline std::shared_ptr<Transform> GetTransform() { return m_gameObjectPtr->GetTransform(); }

	__GET_SET_PROPERTY__(Color, glm::vec4, m_color)
		__GET_SET_BOOLEANPROPERTY__(Realtime, m_isRealtime)
protected:
	glm::vec4 m_color;
	bool m_isRealtime = true;
};


DERIVED_COMPONENT_CLASS(DirectionalLight,Light, ComponentType::ComponentType_Light)
{
public:
	LightProperties GetLightProperties() override
	{
		LightProperties lightProperties;
		lightProperties.isEnabled = m_enabled;
		lightProperties.isLocal = false;
		lightProperties.isSpot = false;
		lightProperties.position = GetTransform()->GetPosition();
		lightProperties.ambient = m_color * 0.1f;
		lightProperties.color = m_color;
		lightProperties.coneDirection = GetTransform()->Forward();
		lightProperties.spotCosCutoff = 0.0f;
		lightProperties.attenuations = glm::zero<glm::vec4>();
		return lightProperties;
	}

private:
	glm::vec3 m_diretion;
};

DERIVED_COMPONENT_CLASS(SpotLight, Light, ComponentType::ComponentType_Light)
{
public:
	LightProperties GetLightProperties() override
	{
		LightProperties lightProperties;
		lightProperties.isEnabled = m_enabled;
		lightProperties.isLocal = true;
		lightProperties.isSpot = true;
		lightProperties.position = GetTransform()->GetPosition();
		lightProperties.ambient = m_color * 0.1f;
		lightProperties.color = m_color;
		lightProperties.coneDirection = GetTransform()->Forward();
		// 表示聚光灯半角
		lightProperties.spotCosCutoff = glm::radians(m_angle / 2);
		lightProperties.attenuations = glm::vec4(m_attenuationStrength, m_constantAttenuation, m_linearAttenuationFactor, m_quadraticAttenuationFactor);
		return lightProperties;
	}

	__GET_SET_PROPERTY__(Angle, float, m_angle)
		__GET_SET_PROPERTY__(ConstantAttenuation, float, m_constantAttenuation)
		__GET_SET_PROPERTY__(LinearAttenuationFactor, float, m_linearAttenuationFactor)
		__GET_SET_PROPERTY__(QuadraticAttenuationFactor, float, m_quadraticAttenuationFactor)
private:
	float m_angle;
	float m_attenuationStrength;
	float m_constantAttenuation;
	float m_linearAttenuationFactor;
	float m_quadraticAttenuationFactor;
};

DERIVED_COMPONENT_CLASS(PointLight, Light, ComponentType::ComponentType_Light)
{
public:
	LightProperties GetLightProperties() override
	{
		LightProperties lightProperties;
		lightProperties.isEnabled = m_enabled;
		lightProperties.isLocal = true;
		lightProperties.isSpot = false;
		lightProperties.position = GetTransform()->GetPosition();
		lightProperties.ambient = m_color * 0.1f;
		lightProperties.color = m_color;
		lightProperties.coneDirection = GetTransform()->Forward();
		// 表示聚光灯半角
		lightProperties.spotCosCutoff = 0.0f;
		lightProperties.attenuations = glm::vec4(m_attenuationStrength, m_constantAttenuation, m_linearAttenuationFactor, m_quadraticAttenuationFactor);
		return lightProperties;
	}

	__GET_SET_PROPERTY__(ConstantAttenuation, float, m_constantAttenuation)
		__GET_SET_PROPERTY__(LinearAttenuationFactor, float, m_linearAttenuationFactor)
		__GET_SET_PROPERTY__(QuadraticAttenuationFactor, float, m_quadraticAttenuationFactor)
private:
	float m_attenuationStrength;
	float m_constantAttenuation;
	float m_linearAttenuationFactor;
	float m_quadraticAttenuationFactor;
};