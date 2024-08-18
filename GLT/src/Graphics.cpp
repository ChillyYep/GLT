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
	transform->SetMatrix(modelMatrix);
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
	transform->SetMatrix(modelMatrix);
	scenePtr->addObject(gameObject);

	componentStateMachine.Setup(renderer);
	while (componentStateMachine.GetComponentLifeStyle() != ComponentLifeCycle::Running)
	{
		componentStateMachine.Tick();
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