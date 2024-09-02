#pragma once
#include <CommonDefine.h>
#include <Renderer.h>
#include <Camera.h>
#include <Light.h>
#include <GameObject.h>
#include <SceneUtility.h>
#include <vector>
#include <stack>
#include <map>

class Scene
{
public:
	Scene()
	{
		m_rootGo = SceneUtility::CreateEmptyGameObject();
	};
	~Scene() {};

	void addObject(std::shared_ptr<GameObject> go, std::shared_ptr<Transform> parent = nullptr);
	void removeObject(GameObject* go);

	std::vector<GameObject*> getObjectList(bool includeInactived = true);

	std::vector<GameObject*> getObjectListIncludeDestroying();

	std::vector<Renderer*> filterRenderers(RenderType renderType, bool includeInactived = true);

	std::vector<std::vector<Renderer*>> filterRenderers(std::vector<RenderType> renderTypes, bool includeInactived = true);

	template<typename T>
	std::vector<std::shared_ptr<T>> getComponents(bool includeInactived = true)
	{
		static_assert(TypeCheck::IsComponentType<T>() && "getComponents : Must be ComponentType!");
		std::vector<std::shared_ptr<T>> comps;
		auto gos = getObjectList(includeInactived);
		for (const auto& go : gos)
		{
			std::shared_ptr<T> compPtr = go->getComponent<T>();
			if (compPtr != nullptr)
			{
				comps.push_back(compPtr);
			}
		}
		return comps;
	}

	std::shared_ptr<Camera> getMainCamera();

	GameObject* getRootGo() { return m_rootGo.get(); }

	bool getActived() { return m_rootGo->getActived(); }

	void setActived(bool actived) { return m_rootGo->setActived(actived); }

		__GET_SET_BOOLEANPROPERTY__(MainScene, m_mainScene)

private:
	bool m_mainScene;

	std::shared_ptr<GameObject> m_rootGo;

	std::map<int, std::shared_ptr<GameObject>> m_allGos;

};