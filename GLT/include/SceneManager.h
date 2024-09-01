#pragma once
#include <Singleton.h>
#include <Scene.h>
#include <stack>
class SceneManager :public Singleton<SceneManager>
{
public:
	~SceneManager() {}
	const std::shared_ptr<Scene> getMainScene() const;
	const std::vector<std::shared_ptr<Scene>> getAllScenes(bool includeInactived = true);

	void addScene(std::shared_ptr<Scene>& scene);
	void removeScene(std::shared_ptr<Scene>& scene);

	bool hasScene(std::shared_ptr<Scene>& scene);

	std::vector<LightProperties> getAffectedLights(std::shared_ptr<Camera>& camera);
private:
	std::vector<std::shared_ptr<Scene>> m_allScenes;
};