#include "Renderer.h"

void Renderer::onEnable()
{
	if (m_mesh != nullptr)
	{
		LogicResourceManagementCentre::getInstance()->addResource(ResourceType::Mesh, m_mesh);
	}
	if (m_material != nullptr)
	{
		auto textures = m_material->getAllTextures();
		for (const auto& texture : textures)
		{
			LogicResourceManagementCentre::getInstance()->addResource(ResourceType::Texture, texture);
		}
	}
}

void Renderer::onDisable()
{
	if (m_mesh != nullptr)
	{
		LogicResourceManagementCentre::getInstance()->destroyResource(ResourceType::Mesh, m_mesh);
	}
	if (m_material != nullptr)
	{
		auto textures = m_material->getAllTextures();
		for (const auto& texture : textures)
		{
			LogicResourceManagementCentre::getInstance()->destroyResource(ResourceType::Texture, texture);
		}
	}
}