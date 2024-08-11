#include "Graphics.h"

void Graphics::drawMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix)
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

void Graphics::drawMeshNow(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix)
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
	drawRequestedMesh(mesh, material, camera, modelMatrix);
}
void Graphics::drawRequestedMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix)
{
	auto resourceIdentifier = ResourceManager::getInstance()->getMeshResource(mesh->getInstanceId());
	if (!resourceIdentifier.isValid())
	{
		return;
	}
	if (resourceIdentifier.isValid())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resourceIdentifier.getEBO());

		glBindVertexArray(resourceIdentifier.getVAO());

		glBindBuffer(GL_ARRAY_BUFFER, resourceIdentifier.getVBO());

		auto mainTexProperty = material->getMainTex();
		auto shader = material->GetShader();
		GLuint program = shader->getShaderProgram();

		if (program > 0 && glIsProgram(program))
		{
			auto lightProperties = SceneManager::getInstance()->getAffectedLights(camera);
			glUseProgram(program);
			Shader::setGlobalMatrix(ShaderPropertyNames::ModelMatrix, modelMatrix);
			Shader::upload(ConstantBufferType::PerPass);
			ShaderUtils::bindBlockForProgram(*shader.get());
			//shader->SetUniformBlock(ShaderPropertyNames::Lights)
			//shader->SetVector(ShaderPropertyNames::Lights, &lightProperties);

			Texture* mainTex = mainTexProperty == nullptr ? nullptr : ((MaterialTextureProperty*)mainTexProperty.get())->GetTexture().get();
			if (mainTex != nullptr)
			{
				auto textureIdentifier = ResourceManager::getInstance()->getTextureResource(mainTex->getInstanceId());
				auto target = Texture::textureType2TextureTarget(textureIdentifier.m_textureType);
				if (target != GL_NONE)
				{
					glBindTextureUnit(0, textureIdentifier.m_texture);
				}
			}
		}
		glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_SHORT, NULL);
	}
}