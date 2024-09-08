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

Bound Renderer::getLocalBound()
{
	if (m_mesh != nullptr)
	{
		return m_mesh->getBound();
	}
	return Bound();
}

Bound Renderer::getWorldBound()
{
	Bound bound = Bound();
	if (m_mesh != nullptr)
	{
		glm::mat4x4 modelMatrix = static_cast<GameObject*>(m_gameObjectPtr)->getTransform()->getModelMatrix();
		auto verticesCount = m_mesh->getVerticesCount();
		std::vector<glm::vec4> newVertices(verticesCount);
		auto oldVertices = m_mesh->getVertices();
		for (size_t i = 0; i < verticesCount; i++)
		{
			newVertices[i] = modelMatrix * oldVertices[i];
		}
		bound = Mesh::computeBound(newVertices.data(), verticesCount);
		return bound;
	}
	return bound;
}