#pragma once
#include <CommonDefine.h>
#include <Mesh.h>
#include <Transform.h>
#include <Component.h>
#include <Material.h>
#include <RenderResourceManager.h>

COMPONENT_CLASS(Renderer, ComponentType::Renderer)
{
public:
	ComponentType getComponentType() override { return ComponentType::Renderer; }
	Renderer() {}
	Renderer(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
	~Renderer() {}

	void onEnable() override;

	void onDisable() override;

	__GET_SET_PROPERTY__(Mesh, std::shared_ptr<Mesh>, m_mesh)

		__GET_SET_PROPERTY__(Material, std::shared_ptr<Material>, m_material)

private:
	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
};
