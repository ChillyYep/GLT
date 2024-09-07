#pragma once
#include <CommonDefine.h>
#include <Mesh.h>
#include <Transform.h>
#include <Material.h>
#include <LogicResourceManagementCentre.h>
#include <FilterSetting.h>
#include <GameObject.h>

COMPONENT_CLASS(Renderer, ComponentType::Renderer)
{
public:
	ComponentType getComponentType() override { return ComponentType::Renderer; }
	Renderer() {}
	Renderer(Mesh * mesh) { m_mesh = mesh; }
	~Renderer() {}

	void onEnable() override;

	void onDisable() override;

	Bound getLocalBound()
	{
		if (m_mesh != nullptr)
		{
			return m_mesh->getBound();
		}
		return Bound();
	}

	Bound getWorldBound()
	{
		if (m_mesh != nullptr)
		{
			glm::mat4x4 modelMatrix = static_cast<GameObject*>(m_gameObjectPtr)->getTransform()->getModelMatrix();
			auto verticesCount = m_mesh->getVerticesCount();
			glm::vec4* newVertices = new glm::vec4[verticesCount];
			auto oldVertices = m_mesh->getVertices();
			for (size_t i = 0; i < verticesCount; i++)
			{
				newVertices[i] = oldVertices[i] * modelMatrix;
			}
			return Mesh::computeBound(newVertices, verticesCount);
		}
		return Bound();
	}

	__GET_SET_PROPERTY__(Mesh, Mesh*, m_mesh)
		__GET_SET_PROPERTY__(RenderType, RenderType, m_renderType)
		__GET_SET_PROPERTY__(Material, std::shared_ptr<Material>, m_material)

private:
	RenderType m_renderType = RenderType::Opaque;
	Mesh* m_mesh;
	std::shared_ptr<Material> m_material;
};
