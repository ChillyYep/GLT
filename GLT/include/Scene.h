#pragma once
#include <CommonDefine.h>
#include <Renderer.h>
#include <Camera.h>
#include <Light.h>
#include <GameObject.h>

class Scene
{
public:
	Scene() {};
	~Scene() {};

	void addObject(std::shared_ptr<GameObject> go);

	void removeObject(std::shared_ptr<GameObject> go);

	inline std::vector<std::shared_ptr<GameObject>> getObjectList() { return m_gos; }

	std::vector<std::shared_ptr<Light>> getLights();

	std::shared_ptr<Camera> getMainCamera();

	__GET_SET_PROPERTY__(Actived, bool, m_actived)
		__GET_SET_BOOLEANPROPERTY__(MainScene, m_mainScene)

private:
	bool m_mainScene;
	bool m_actived = true;
	std::vector<std::shared_ptr<GameObject>> m_gos;

};