#include "Renderer.h"

void Renderer::onEnable()
{
	for (int i = 0;i < m_meshes.size();++i)
	{
		LogicResourceManager::getInstance()->addResource(ResourceType::Mesh, m_meshes[i]);
	}
	for (int i = 0;i < m_materials.size();++i)
	{
		auto textures = m_materials[i]->getAllTextures();
		for (const auto& texture : textures)
		{
			LogicResourceManager::getInstance()->addResource(ResourceType::Texture, texture);
		}
	}
}

void Renderer::onDisable()
{
	for (int i = 0;i < m_meshes.size();++i)
	{
		LogicResourceManager::getInstance()->destroyResource(ResourceType::Mesh, m_meshes[i]);
	}
	for (int i = 0;i < m_materials.size();++i)
	{
		auto textures = m_materials[i]->getAllTextures();
		for (const auto& texture : textures)
		{
			LogicResourceManager::getInstance()->destroyResource(ResourceType::Texture, texture);
		}
	}
}

Bound Renderer::getLocalBound()
{
	Bound bound = m_meshes.size() > 0 ? m_meshes[0]->getBound() : Bound();
	for (int i = 1;i < m_meshes.size();++i)
	{
		bound = bound.Extends(m_meshes[i]->getBound());
	}
	return bound;
}

Bound Renderer::getWorldBound()
{
	Bound bound = m_meshes.size() > 0 ? m_meshes[0]->getBound() : Bound();
	for (int i = 1;i < m_meshes.size();++i)
	{
		glm::mat4x4 modelMatrix = static_cast<GameObject*>(m_gameObjectPtr)->getTransform()->getModelMatrix();
		auto verticesCount = m_meshes[i]->getVerticesCount();
		std::vector<glm::vec4> newVertices(verticesCount);
		auto oldVertices = m_meshes[i]->getVertices();
		for (size_t i = 0; i < verticesCount; i++)
		{
			newVertices[i] = modelMatrix * oldVertices[i];
		}
		bound = bound.Extends(SubMesh::computeBound(newVertices.data(), verticesCount));
	}
	return bound;
}