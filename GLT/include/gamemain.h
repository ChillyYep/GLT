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
			auto objects = scene->getObjectList();
			for (const auto& obj : objects)
			{
				for (const auto& comp : obj->getComponents())
				{
					m_componentStateMachine.setup(comp);
					m_componentStateMachine.Tick();
				}
			}
		}
	}

private:
	ComponentStateMachine m_componentStateMachine;
};