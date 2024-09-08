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

	Bound getLocalBound();

	Bound getWorldBound();

	__GET_SET_PROPERTY__(Mesh, Mesh*, m_mesh)
		__GET_SET_PROPERTY__(RenderType, RenderType, m_renderType)
		__GET_SET_PROPERTY__(Material, std::shared_ptr<Material>, m_material)

private:
	RenderType m_renderType = RenderType::Opaque;
	Mesh* m_mesh;
	std::shared_ptr<Material> m_material;
};
