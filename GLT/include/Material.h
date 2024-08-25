#pragma once
#include <CommonDefine.h>
#include <map>
#include <Texture.h>
#include <Shader.h>
#include <ShaderPropertyNames.h>

enum class MaterialPropertyType {
	Texture,
	Bool,
	Int,
	Float,
	Vector4,
	Matrix4
};
class MaterialProperty
{
public:
	MaterialProperty() {}
	~MaterialProperty() {}
	virtual MaterialPropertyType getMaterialPropertyType() = 0;

private:
};

class MaterialTextureProperty :public MaterialProperty
{
public:
	MaterialTextureProperty(std::shared_ptr<Texture> texPtr) :m_texPtr(texPtr) {}
	~MaterialTextureProperty() {}
	MaterialPropertyType getMaterialPropertyType() override { return MaterialPropertyType::Texture; }
	__GET_SET_PROPERTY__(Texture, std::shared_ptr<Texture>, m_texPtr)

private:
	std::shared_ptr<Texture> m_texPtr;
};

class MaterialVector4Property :public MaterialProperty
{
public:
	MaterialVector4Property(glm::vec4 vec4) :m_vec4(vec4) {}
	~MaterialVector4Property() {}
	MaterialPropertyType getMaterialPropertyType() override { return MaterialPropertyType::Vector4; }
	__GET_SET_PROPERTY__(Value, glm::vec4, m_vec4)

private:
	glm::vec4 m_vec4;
};

class MaterialFloatProperty :public MaterialProperty
{
public:
	MaterialFloatProperty(float value) :m_value(value) {}
	~MaterialFloatProperty() {}
	MaterialPropertyType getMaterialPropertyType() override { return MaterialPropertyType::Float; }
	__GET_SET_PROPERTY__(Value, float, m_value)
private:
	float m_value;
};

class MaterialIntProperty :public MaterialProperty
{
public:
	MaterialIntProperty(int value) :m_value(value) {}
	~MaterialIntProperty() {}
	MaterialPropertyType getMaterialPropertyType() override { return MaterialPropertyType::Int; }
	__GET_SET_PROPERTY__(Value, int, m_value)

private:
	int m_value;
};

class MaterialBoolProperty :public MaterialProperty
{
public:
	MaterialBoolProperty(bool value) :m_value(value) {}
	~MaterialBoolProperty() {}
	MaterialPropertyType getMaterialPropertyType() override { return MaterialPropertyType::Bool; }
	__GET_SET_PROPERTY__(Value, bool, m_value)
private:
	bool m_value;
};

class MaterialMatrix4Property :public MaterialProperty
{
public:
	MaterialMatrix4Property(glm::mat4 value) :m_value(value) {}
	~MaterialMatrix4Property() {}
	MaterialPropertyType getMaterialPropertyType() override { return MaterialPropertyType::Matrix4; }
	__GET_SET_PROPERTY__(Value, glm::mat4, m_value)
private:
	glm::mat4 m_value;
};


class Material
{
public:
	Material(std::shared_ptr<Shader> shader) :m_shader(shader) {}
	~Material() {}
	inline void setProperty(std::string name, std::shared_ptr<MaterialProperty> property) { m_properties[name] = property; }
	inline std::shared_ptr<MaterialProperty> getProperty(std::string name) const
	{
		const auto ptr = m_properties.find(name);
		if (ptr != m_properties.end())
		{
			return ptr->second;
		}
		return std::shared_ptr<MaterialProperty>(nullptr);
	}
	inline std::shared_ptr<MaterialProperty> getMainTex() const
	{
		return getProperty(ShaderPropertyNames::MainTex);
	}
	inline std::vector<std::shared_ptr<Texture>> getAllTextures()
	{
		std::vector<std::shared_ptr<Texture>> textures;
		for (const auto& propertyPair : m_properties)
		{
			if (propertyPair.second->getMaterialPropertyType() == MaterialPropertyType::Texture)
			{
				textures.push_back(((MaterialTextureProperty*)propertyPair.second.get())->getTexture());
			}
		}
		return textures;
	}

	__GET_SET_PROPERTY__(Shader, std::shared_ptr<Shader>, m_shader)
private:
	std::map<std::string, std::shared_ptr<MaterialProperty>> m_properties;
	std::shared_ptr<Shader> m_shader;
};
