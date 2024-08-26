#include "Material.h"

std::vector<std::shared_ptr<Texture>> Material::getAllTextures()
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
