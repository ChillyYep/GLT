#pragma once
#include <Scene.h>
class SceneManager
{
public:
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	inline static SceneManager& GetInstance() {

		static SceneManager instance;
		return instance;
	}
	~SceneManager() {}
	inline const std::shared_ptr<Scene> GetMainScene() const {
		for (int i = 0;i < m_allScenes.size();++i)
		{
			if (m_allScenes[i]->GetMainScene() && m_allScenes[i]->GetActived())
			{
				return m_allScenes[i];
			}
		}
		return nullptr;
	}
	inline const std::vector<std::shared_ptr<Scene>> GetAllScenes(bool includeInactived = true) {
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

	inline void AddScene(std::shared_ptr<Scene>& scene)
	{
		if (!HasScene(scene))
		{
			m_allScenes.push_back(scene);
		}
	}

	inline void RemoveScene(std::shared_ptr<Scene>& scene)
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

	inline bool HasScene(std::shared_ptr<Scene>& scene)
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

	inline std::vector<LightProperties> GetAffectedLights(std::shared_ptr<Camera>& camera)
	{
		auto allScenes = SceneManager::GetInstance().GetAllScenes(false);
		std::vector<LightProperties> lights;
		for (const auto& scene : allScenes)
		{
			auto objs = scene->GetObjectList();
			for (const auto& obj : objs)
			{
				auto light = obj->GetComponent<Light>();
				if (light != nullptr && light->IsRealtime())
				{
					auto lightProperties = light->GetLightProperties();
					lights.push_back(lightProperties);
				}
			}
		}
		return lights;
	}
private:
	SceneManager() {}
	std::vector<std::shared_ptr<Scene>> m_allScenes;
};