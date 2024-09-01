#include "Scene.h"
void Scene::addObject(std::shared_ptr<GameObject> go, std::shared_ptr<Transform> parent)
{
	assert(m_allGos.find(go->getInstanceId()) == m_allGos.end() && !go->IsDestroyed());
	m_allGos[go->getInstanceId()] = go;
	if (parent == nullptr)
	{
		m_rootGo->getTransform()->addChild(go->getTransform().get());
	}
	else {
		parent->addChild(go->getTransform().get());
	}
}
void Scene::removeObject(GameObject* go)
{
	if (!go->IsDestroyed())
	{
		go->destroy();
	}
	assert(m_allGos.find(go->getInstanceId()) != m_allGos.end());
	m_allGos.erase(go->getInstanceId());
}
std::shared_ptr<Camera> Scene::getMainCamera()
{
	for (const auto& camera : getComponents<Camera>(false))
	{
		if (camera == nullptr || !camera->getMainCameraFlag())
		{
			continue;
		}
		return camera;
	}
	return nullptr;
}
