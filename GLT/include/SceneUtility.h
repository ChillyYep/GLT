#pragma once
#include <CommonDefine.h>
#include <GameObject.h>
#include <Transform.h>
#include <Mesh.h>
#include <Material.h>
#include <Renderer.h>
class SceneUtility
{
public:
	inline static std::shared_ptr<GameObject> createEmptyGameObject()
	{
		auto gameObject = std::shared_ptr<GameObject>(new GameObject());
		gameObject->addComponent<Transform>();
		return gameObject;
	}
	inline static std::shared_ptr<GameObject> createMeshGameObject(SubMesh* mesh, const std::shared_ptr<Material>& material)
	{
		auto gameObject = createEmptyGameObject();
		auto renderer = gameObject->addComponent<Renderer>();
		renderer->addMesh(mesh, material);
		return gameObject;
	}
	inline static std::shared_ptr<GameObject> createMeshGameObject(std::vector<SubMesh*> meshes, std::vector<std::shared_ptr<Material>>& materials)
	{
		auto gameObject = createEmptyGameObject();
		auto renderer = gameObject->addComponent<Renderer>();
		renderer->addMeshes(meshes, materials);
		return gameObject;
	}
private:

};