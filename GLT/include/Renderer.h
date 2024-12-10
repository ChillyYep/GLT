#pragma once
#include <CommonDefine.h>
#include <Mesh.h>
#include <Transform.h>
#include <Material.h>
#include <LogicResourceManager.h>
#include <FilterSetting.h>
#include <GameObject.h>

COMPONENT_CLASS(Renderer, ComponentType::Renderer)
{
public:
	ComponentType getComponentType() override { return ComponentType::Renderer; }
	Renderer() {}
	Renderer(std::vector<SubMesh*> meshes, std::vector<std::shared_ptr<Material>> materials) { m_meshes = meshes; m_materials = materials; }
	~Renderer() {}

	void onEnable() override;

	void onDisable() override;

	Bound getLocalBound();

	Bound getWorldBound();

	void addMesh(SubMesh * mesh, std::shared_ptr<Material> material)
	{
		m_meshes.push_back(mesh);
		m_materials.push_back(material);
	}

	void addMeshes(std::vector<SubMesh*>&meshes, std::vector<std::shared_ptr<Material>>&materials)
	{
		assert(meshes.size() == materials.size());
		for (int i = 0; i < meshes.size(); ++i)
		{
			m_meshes.push_back(meshes[i]);
			m_materials.push_back(materials[i]);
		}
	}

	inline SubMesh* getMesh(int index)
	{
		if (index < m_meshes.size() && index >= 0)
		{
			return m_meshes[index];
		}
		return nullptr;
	}

	inline std::shared_ptr<Material> getMaterial(int index)
	{
		if (index < m_materials.size() && index >= 0)
		{
			return m_materials[index];
		}
		return nullptr;
	}


	inline std::vector<SubMesh*> getMeshes() { return m_meshes; }
	inline std::vector<std::shared_ptr<Material>> getMaterials() { return m_materials; }
	inline size_t getMeshCount() { return m_meshes.size(); }

	inline RenderType getRenderType() const
	{
		int renderType = 0;
		for (int i = 0; i < m_materials.size(); ++i)
		{
			if (m_materials[i] == nullptr)
			{
				continue;
			}
			renderType |= (int)m_materials[i]->getRenderType();
		}
		return (RenderType)renderType;
	}

private:
	std::vector<SubMesh*> m_meshes;
	std::vector<std::shared_ptr<Material>> m_materials;
};
