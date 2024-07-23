#pragma once
#include <CommonDefine.h>
#include <Renderer.h>
#include <Camera.h>
#include <ResourceManager.h>
#include <MeshManagementCentre.h>
#include <TextureManagementCentre.h>
#include <Light.h>
#include <GameObject.h>

class Scene
{
public:
	Scene() {};
	~Scene() {};

	void AddObject(std::shared_ptr<GameObject> go);

	void RemoveObject(std::shared_ptr<GameObject> go);

	inline std::vector<std::shared_ptr<GameObject>> GetObjectList() { return m_gos; }

	inline std::vector<std::shared_ptr<Light>> GetLights()
	{
		std::vector<std::shared_ptr<Light>> lights;
		for (const auto& go : m_gos)
		{
			auto light = go->GetComponent<Light>();
			if (light == nullptr)
			{
				continue;
			}
			lights.push_back(light);
		}
		return lights;
	}

	inline std::shared_ptr<Camera> GetMainCamera()
	{
		for (const auto& go : m_gos)
		{
			auto camera = go->GetComponent<Camera>();
			if (camera == nullptr || !camera->GetMainCameraFlag())
			{
				continue;
			}
			return camera;
		}
		return nullptr;
	}

	__GET_SET_PROPERTY__(Actived, bool, m_actived)
		__GET_SET_PROPERTY__(MainScene, bool, m_mainScene)

private:
	bool m_mainScene;
	bool m_actived = true;
	std::vector<std::shared_ptr<GameObject>> m_gos;

};