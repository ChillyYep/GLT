#pragma once
#include <CommonDefine.h>
#include <Mesh.h>
#include <Transform.h>
#include <Component.h>
#include <Material.h>
#include <ResourceManager.h>

COMPONENT_CLASS(Renderer, ComponentType::Renderer)
{
public:
	ComponentType getComponentType() override { return ComponentType::Renderer; }
	Renderer() {}
	Renderer(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
	~Renderer() {}

	void OnEnable() override
	{
		if (m_mesh != nullptr)
		{
			ResourceManager::getInstance()->getMeshManagementCentre().add(m_mesh);
		}
		if (m_material != nullptr)
		{
			auto textures = m_material->getAllTextures();
			for (const auto& texture : textures)
			{
				ResourceManager::getInstance()->getTextureManagementCentre().add(texture);
			}
		}
	}

	void OnDisable() override
	{
		if (m_mesh != nullptr)
		{
			ResourceManager::getInstance()->getMeshManagementCentre().remove(m_mesh);
		}
		if (m_material != nullptr)
		{
			auto textures = m_material->getAllTextures();
			for (const auto& texture : textures)
			{
				ResourceManager::getInstance()->getTextureManagementCentre().remove(texture);
			}
		}
	}

	__GET_SET_PROPERTY__(Mesh, std::shared_ptr<Mesh>, m_mesh)

		__GET_SET_PROPERTY__(Material, std::shared_ptr<Material>, m_material)

private:
	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
};
