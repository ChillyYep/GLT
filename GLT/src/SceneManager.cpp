#include "SceneManager.h"

const std::shared_ptr<Scene> SceneManager::getMainScene() const {
	for (int i = 0; i < m_allScenes.size(); ++i)
	{
		if (m_allScenes[i]->IsMainScene() && m_allScenes[i]->getActived())
		{
			return m_allScenes[i];
		}
	}
	return nullptr;
}
const std::vector<std::shared_ptr<Scene>> SceneManager::getAllScenes(bool includeInactived) {
	std::vector<std::shared_ptr<Scene>> allScenes;
	for (int i = 0; i < m_allScenes.size(); ++i)
	{
		if (!includeInactived && !m_allScenes[i]->getActived())
		{
			continue;
		}
		allScenes.push_back(m_allScenes[i]);
	}
	return allScenes;
}

void SceneManager::addScene(std::shared_ptr<Scene>& scene)
{
	if (!hasScene(scene))
	{
		m_allScenes.push_back(scene);
	}
}

void SceneManager::removeScene(std::shared_ptr<Scene>& scene)
{
	for (int i = 0; i < m_allScenes.size(); ++i)
	{
		if (m_allScenes[i] == scene)
		{
			for (int j = i; j < m_allScenes.size() - 1; ++j)
			{
				m_allScenes[j] = m_allScenes[j + 1];
			}
			m_allScenes.pop_back();
			return;
		}
	}
}

bool SceneManager::hasScene(std::shared_ptr<Scene>& scene)
{
	for (int i = 0; i < m_allScenes.size(); ++i)
	{
		if (m_allScenes[i] == scene)
		{
			return true;
		}
	}
	return false;
}

std::vector<LightProperties> SceneManager::getAffectedLights(std::shared_ptr<Camera>& camera)
{
	auto allScenes = SceneManager::getInstance()->getAllScenes(false);
	std::vector<LightProperties> allLightProperties;
	for (const auto& scene : allScenes)
	{
		auto lights = scene->getComponents<Light>(false);
		for (const auto& light : lights)
		{
			if (light != nullptr && light->IsRealtime() && light->getEnable())
			{
				auto lightProperties = light->getLightProperties();
				allLightProperties.push_back(lightProperties);
			}
		}
	}
	return allLightProperties;
}