#pragma once
#include <Singleton.h>
#include <Scene.h>
class SceneManager :public Singleton<SceneManager>
{
public:
	~SceneManager() {}
	inline const std::shared_ptr<Scene> getMainScene() const {
		for (int i = 0;i < m_allScenes.size();++i)
		{
			if (m_allScenes[i]->IsMainScene() && m_allScenes[i]->GetActived())
			{
				return m_allScenes[i];
			}
		}
		return nullptr;
	}
	inline const std::vector<std::shared_ptr<Scene>> getAllScenes(bool includeInactived = true) {
		std::vector<std::shared_ptr<Scene>> allScenes;
		for (int i = 0;i < m_allScenes.size();++i)
		{
			if (!includeInactived && !m_allScenes[i]->GetActived())
			{
				continue;
			}
			allScenes.push_back(m_allScenes[i]);
		}
		return allScenes;
	}

	inline void addScene(std::shared_ptr<Scene>& scene)
	{
		if (!hasScene(scene))
		{
			m_allScenes.push_back(scene);
		}
	}

	inline void removeScene(std::shared_ptr<Scene>& scene)
	{
		for (int i = 0;i < m_allScenes.size();++i)
		{
			if (m_allScenes[i] == scene)
			{
				for (int j = i;j < m_allScenes.size() - 1;++j)
				{
					m_allScenes[j] = m_allScenes[j + 1];
				}
				m_allScenes.pop_back();
				return;
			}
		}
	}

	inline bool hasScene(std::shared_ptr<Scene>& scene)
	{
		for (int i = 0;i < m_allScenes.size();++i)
		{
			if (m_allScenes[i] == scene)
			{
				return true;
			}
		}
		return false;
	}

	inline std::vector<LightProperties> getAffectedLights(std::shared_ptr<Camera>& camera)
	{
		auto allScenes = SceneManager::getInstance()->getAllScenes(false);
		std::vector<LightProperties> lights;
		for (const auto& scene : allScenes)
		{
			auto objs = scene->getObjectList();
			for (const auto& obj : objs)
			{
				auto light = obj->getComponent<Light>();
				if (light != nullptr && light->IsRealtime())
				{
					auto lightProperties = light->getLightProperties();
					lights.push_back(lightProperties);
				}
			}
		}
		return lights;
	}
private:
	std::vector<std::shared_ptr<Scene>> m_allScenes;
};