#include "Renderer.h"

void Renderer::onEnable()
{
	if (m_mesh != nullptr)
	{
		RenderResourceManager::getInstance()->getMeshManagementCentre().add(m_mesh);
	}
	if (m_material != nullptr)
	{
		auto textures = m_material->getAllTextures();
		for (const auto& texture : textures)
		{
			RenderResourceManager::getInstance()->getTextureManagementCentre().add(texture);
		}
	}
}

void Renderer::onDisable()
{
	if (m_mesh != nullptr)
	{
		RenderResourceManager::getInstance()->getMeshManagementCentre().remove(m_mesh);
	}
	if (m_material != nullptr)
	{
		auto textures = m_material->getAllTextures();
		for (const auto& texture : textures)
		{
			RenderResourceManager::getInstance()->getTextureManagementCentre().remove(texture);
		}
	}
}