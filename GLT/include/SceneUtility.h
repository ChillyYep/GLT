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
	inline static std::shared_ptr<GameObject> CreateEmptyGameObject()
	{
		auto gameObject = std::shared_ptr<GameObject>(new GameObject());
		gameObject->AddComponent<Transform>();
		return gameObject;
	}
	inline static std::shared_ptr<GameObject> CreateMeshGameObject(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material)
	{
		auto gameObject = CreateEmptyGameObject();
		auto renderer = gameObject->AddComponent<Renderer>();
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);
		return gameObject;
	}
private:

};