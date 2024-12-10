#pragma once
#include <CommonDefine.h>
#include <map>
#include <Texture.h>
#include <Shader.h>
#include <ShaderPropertyNames.h>
#include <FilterSetting.h>

enum class MaterialPropertyType {
	Texture,
	Bool,
	Int,
	Int2,
	Int3,
	Int4,
	Float,
	Vector2,
	Vector3,
	Vector4,
	Matrix2,
	Matrix2x3,
	Matrix2x4,
	Matrix3x2,
	Matrix3,
	Matrix3x4,
	Matrix4x2,
	Matrix4x3,
	Matrix4,
	Vector4Array,
};
class MaterialProperty
{
public:
	MaterialProperty() {}
	~MaterialProperty() {}
	virtual MaterialPropertyType getMaterialPropertyType() = 0;

private:
};

#define MaterialCommonProperty(CLASSNAME,VALUETYPE,VALUEENUM) \
class CLASSNAME :public MaterialProperty	\
{	\
public:	\
	CLASSNAME(VALUETYPE value) :m_value(value) {}	\
	~CLASSNAME() {}	\
	MaterialPropertyType getMaterialPropertyType() override { return VALUEENUM; }	\
	__GET_SET_PROPERTY__(Value, VALUETYPE, m_value)	\
\
private:	\
	VALUETYPE m_value;	\
};	\

MaterialCommonProperty(MaterialTextureProperty, Texture*, MaterialPropertyType::Texture)
MaterialCommonProperty(MaterialFloatProperty, float, MaterialPropertyType::Float)
MaterialCommonProperty(MaterialVector2Property, glm::vec2, MaterialPropertyType::Vector2)
MaterialCommonProperty(MaterialVector3Property, glm::vec3, MaterialPropertyType::Vector3)
MaterialCommonProperty(MaterialVector4Property, glm::vec4, MaterialPropertyType::Vector4)
MaterialCommonProperty(MaterialVector4ArrayProperty, std::vector<glm::vec4>, MaterialPropertyType::Vector4Array)
MaterialCommonProperty(MaterialBoolProperty, bool, MaterialPropertyType::Bool)
MaterialCommonProperty(MaterialIntProperty, int, MaterialPropertyType::Int)
MaterialCommonProperty(MaterialInt2Property, glm::ivec2, MaterialPropertyType::Int2)
MaterialCommonProperty(MaterialInt3Property, glm::ivec3, MaterialPropertyType::Int3)
MaterialCommonProperty(MaterialInt4Property, glm::ivec4, MaterialPropertyType::Int4)
MaterialCommonProperty(MaterialMatrix2Property, glm::mat2, MaterialPropertyType::Matrix2)
MaterialCommonProperty(MaterialMatrix2x3Property, glm::mat2x3, MaterialPropertyType::Matrix2x3)
MaterialCommonProperty(MaterialMatrix2x4Property, glm::mat2x4, MaterialPropertyType::Matrix2x4)
MaterialCommonProperty(MaterialMatrix3x2Property, glm::mat3x2, MaterialPropertyType::Matrix3x2)
MaterialCommonProperty(MaterialMatrix3Property, glm::mat3, MaterialPropertyType::Matrix3)
MaterialCommonProperty(MaterialMatrix3x4Property, glm::mat3x4, MaterialPropertyType::Matrix3x4)
MaterialCommonProperty(MaterialMatrix4x2Property, glm::mat4x2, MaterialPropertyType::Matrix4x2)
MaterialCommonProperty(MaterialMatrix4x3Property, glm::mat4x3, MaterialPropertyType::Matrix4x3)
MaterialCommonProperty(MaterialMatrix4Property, glm::mat4, MaterialPropertyType::Matrix4)


class Material :public Object
{
public:
	Material(std::shared_ptr<Shader> shader) :m_shader(shader) {}
	~Material() {}
	inline void setProperty(std::string name, std::shared_ptr<MaterialProperty> property)
	{
		m_properties[name] = property;
	}
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
	std::vector<Texture*> getAllTextures();

	__GET_SET_PROPERTY__(Shader, std::shared_ptr<Shader>, m_shader)
		__GET_SET_PROPERTY__(RenderType, RenderType, m_renderType)
private:
	std::map<std::string, std::shared_ptr<MaterialProperty>> m_properties;
	std::shared_ptr<Shader> m_shader;
	RenderType m_renderType;
};
