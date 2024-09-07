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

std::vector<GameObject*> Scene::getObjectList(bool includeInactived)
{
	std::vector<GameObject*> gos;
	std::stack<GameObject*> nodes;
	nodes.push(m_rootGo.get());
	while (nodes.size() > 0)
	{
		auto curNode = nodes.top();
		nodes.pop();
		if (curNode->getInstanceId() != m_rootGo->getInstanceId())
		{
			gos.push_back(curNode);
		}
		auto children = curNode->getTransform()->getChildren();
		for (int i = (int)children.size() - 1; i >= 0; i--)
		{
			auto compOwner = children[i]->getGameObject();
			auto go = static_cast<GameObject*>(compOwner);
			if ((includeInactived || compOwner->getActived()) && go != nullptr)
			{
				nodes.push(go);
			}
		}
	}
	return gos;
}

std::vector<GameObject*> Scene::getObjectListIncludeDestroying()
{
	std::vector<GameObject*> gos;
	for (const auto& goPair : m_allGos)
	{
		gos.push_back(goPair.second.get());
	}
	return gos;
}

std::vector<Renderer*> Scene::filterRenderers(RenderType renderType, bool includeInactived)
{
	std::vector<Renderer*> renderers;
	auto rendererPtrs = getComponents<Renderer>(includeInactived);
	for (const auto& rendererPtr : rendererPtrs)
	{
		if (((int)rendererPtr->getRenderType() & (int)renderType) > 0)
		{
			renderers.push_back(rendererPtr.get());
		}
	}
	return renderers;
}

std::vector<std::vector<Renderer*>> Scene::filterRenderers(std::vector<RenderType> renderTypes, bool includeInactived)
{
	std::vector<std::vector<Renderer*>> renderers(renderTypes.size());
	auto rendererPtrs = getComponents<Renderer>(includeInactived);
	for (const auto& rendererPtr : rendererPtrs)
	{
		for (int i = 0; i < renderTypes.size(); ++i)
		{
			if (((int)rendererPtr->getRenderType() & (int)renderTypes[i]) > 0)
			{
				renderers[i].push_back(rendererPtr.get());
			}
		}
	}
	return renderers;
}