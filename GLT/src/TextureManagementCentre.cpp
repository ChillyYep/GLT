#include "TextureManagementCentre.h"

void TextureManagementCentre::onSubmit()
{
	for (const auto& texturePair : m_allObjects)
	{
		auto& textureItem = texturePair.second;
		auto& texture = textureItem.m_target;
		if (!texture->getCanReadWrite())
		{
			unloadTexture(texture.get());
		}
	}
}