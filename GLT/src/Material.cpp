#include "Material.h"

std::vector<Texture*> Material::getAllTextures()
{
	std::vector<Texture*> textures;
	for (const auto& propertyPair : m_properties)
	{
		if (propertyPair.second->getMaterialPropertyType() == MaterialPropertyType::Texture)
		{
			textures.push_back(((MaterialTextureProperty*)propertyPair.second.get())->getValue());
		}
	}
	return textures;
}
