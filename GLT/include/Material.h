#pragma once
#include <CommonDefine.h>
#include <map>
#include <Texture.h>
#include <Shader.h>
#include <ShaderPropertyNames.h>

enum MaterialPropertyType {
	MaterialProperty_Texture
};
class MaterialProperty
{
public:
	MaterialProperty() {}
	~MaterialProperty() {}
	virtual MaterialPropertyType GetMaterialPropertyType() = 0;

private:
};

class MaterialTextureProperty :public MaterialProperty
{
public:
	MaterialTextureProperty(std::shared_ptr<Texture> texPtr) :m_texPtr(texPtr) {}
	~MaterialTextureProperty() {}
	MaterialPropertyType GetMaterialPropertyType() override { return MaterialPropertyType::MaterialProperty_Texture; }
	__GET_SET_PROPERTY__(Texture, std::shared_ptr<Texture>, m_texPtr)

private:
	std::shared_ptr<Texture> m_texPtr;
};

class Material
{
public:
	Material(std::shared_ptr<Shader> shader) :m_shader(shader) {}
	~Material() {}
	inline void SetProperty(std::string name, std::shared_ptr<MaterialProperty> property) { m_properties[name] = property; }
	inline std::shared_ptr<MaterialProperty> GetProperty(std::string name) const
	{
		const auto ptr = m_properties.find(name);
		if (ptr != m_properties.end())
		{
			return ptr->second;
		}
		return std::shared_ptr<MaterialProperty>(nullptr); 
	}
	inline std::shared_ptr<MaterialProperty> GetMainTex() const
	{ 
		return GetProperty(ShaderPropertyNames::MainTex);
	}
	inline std::vector<std::shared_ptr<Texture>> GetAllTextures()
	{
		std::vector<std::shared_ptr<Texture>> textures;
		for (const auto& propertyPair : m_properties)
		{
			if (propertyPair.second->GetMaterialPropertyType() == MaterialProperty_Texture)
			{
				textures.push_back(((MaterialTextureProperty*)propertyPair.second.get())->GetTexture());
			}
		}
		return textures;
	}

	__GET_SET_PROPERTY__(Shader, std::shared_ptr<Shader>, m_shader)
private:
	std::map<std::string, std::shared_ptr<MaterialProperty>> m_properties;
	std::shared_ptr<Shader> m_shader;
};
