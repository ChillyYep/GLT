#pragma once
#include <Scene.h>
class GameMain
{
public:
	GameMain() {}
	~GameMain() {}
	void run(std::vector<std::shared_ptr<Scene>>& allScenes)
	{
		for (int i = 0;i < allScenes.size();++i)
		{
			auto scene = allScenes[i];
			auto objects = scene->getObjectListIncludeDestroying();
			for (const auto& obj : objects)
			{
				if (obj->isDestroyed())
				{
					scene->removeObject(obj);
				}
				else {
					for (const auto& comp : obj->getComponents())
					{
						m_componentStateMachine.setup(comp.get());
						m_componentStateMachine.tick();
					}
				}

			}
		}
	}

private:
	ComponentStateMachine m_componentStateMachine;
};