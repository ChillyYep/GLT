#include "TextureManagementCentre.h"

void TextureManagementCentre::OnSubmit()
{
	for (const auto& texturePair : m_allObjects)
	{
		auto& textureItem = texturePair.second;
		auto& texture = textureItem.m_target;
		if (!texture->GetCanReadWrite())
		{
			UnloadTexture(texture.get());
		}
	}
}