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

	std::vector<GameObject*> getObjectList(bool includeInactived = true)
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
			for (int i = (int)children.size() - 1;i >= 0;i--)
			{
				auto go = children[i]->getGameObject();
				if (includeInactived || go->getActived())
				{
					nodes.push(go);
				}
			}
		}
		return gos;
	}

	std::vector<GameObject*> getObjectListIncludeDestroying()
	{
		std::vector<GameObject*> gos;
		for (const auto& goPair : m_allGos)
		{
			gos.push_back(goPair.second.get());
		}
		return gos;
	}

	template<typename T>
	std::vector<std::shared_ptr<T>> getComponents(bool includeInactived = true)
	{
		static_assert(TypeCheck::IsComponentType<T>() && "必须传入组件类型");
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

	__GET_SET_PROPERTY__(Actived, bool, m_actived)
		__GET_SET_BOOLEANPROPERTY__(MainScene, m_mainScene)

private:
	bool m_mainScene;
	bool m_actived = true;
	std::shared_ptr<GameObject> m_rootGo;

	std::map<int, std::shared_ptr<GameObject>> m_allGos;

};