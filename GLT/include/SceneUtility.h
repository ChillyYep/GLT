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
	inline static std::shared_ptr<GameObject> createMeshGameObject(Mesh* mesh, const std::shared_ptr<Material>& material)
	{
		auto gameObject = createEmptyGameObject();
		auto renderer = gameObject->addComponent<Renderer>();
		renderer->setMesh(mesh);
		renderer->setMaterial(material);
		return gameObject;
	}
private:

};