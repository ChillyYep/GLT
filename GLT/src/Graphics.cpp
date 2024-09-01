#include "Graphics.h"
RenderPipeline* Graphics::m_pipeline;

void Graphics::drawMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, const glm::mat4x4& modelMatrix)
{
	auto scenePtr = SceneManager::getInstance()->getMainScene();
	if (scenePtr == nullptr)
	{
		return;
	}
	auto gameObject = SceneUtility::CreateMeshGameObject(mesh, material);
	auto transform = gameObject->getComponent<Transform>();
	transform->setModelMatrix(modelMatrix);
	scenePtr->addObject(gameObject);
}

void Graphics::drawMeshNow(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, const glm::mat4x4& modelMatrix)
{
	ComponentStateMachine componentStateMachine;
	auto scenePtr = SceneManager::getInstance()->getMainScene();
	if (scenePtr == nullptr)
	{
		return;
	}
	auto gameObject = SceneUtility::CreateMeshGameObject(mesh, material);
	auto transform = gameObject->getComponent<Transform>();
	auto renderer = gameObject->getComponent<Renderer>();
	transform->setModelMatrix(modelMatrix);
	scenePtr->addObject(gameObject);

	componentStateMachine.setup(renderer.get());
	while (componentStateMachine.getComponentLifeStyle() != ComponentLifeCycle::Running)
	{
		componentStateMachine.tick();
	}
	drawRequestedMesh(mesh, material, modelMatrix);
}
void Graphics::drawRequestedMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, const glm::mat4x4& modelMatrix)
{
	if (m_pipeline != nullptr)
	{
		m_pipeline->drawMesh(mesh.get(), material.get(), modelMatrix);
	}
}