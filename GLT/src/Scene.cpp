#include "Scene.h"
void Scene::addObject(std::shared_ptr<GameObject> go)
{
	m_gos.push_back(go);
}

void Scene::removeObject(std::shared_ptr<GameObject> go)
{
	for (int i = 0;i < m_gos.size();++i)
	{
		if (m_gos[i]->getInstanceId() == go->getInstanceId())
		{
			for (int j = i;j < m_gos.size() - 1;++j)
			{
				m_gos[j] = m_gos[j + 1];
			}
			break;
		}
	}
	m_gos.pop_back();
}


std::vector<std::shared_ptr<Light>> Scene::getLights()
{
	std::vector<std::shared_ptr<Light>> lights;
	for (const auto& go : m_gos)
	{
		auto light = go->getComponent<Light>();
		if (light == nullptr)
		{
			continue;
		}
		lights.push_back(light);
	}
	return lights;
}

std::shared_ptr<Camera> Scene::getMainCamera()
{
	for (const auto& go : m_gos)
	{
		auto camera = go->getComponent<Camera>();
		if (camera == nullptr || !camera->getMainCameraFlag())
		{
			continue;
		}
		return camera;
	}
	return nullptr;
}
