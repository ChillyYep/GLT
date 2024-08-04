#include "Graphics.h"

void Graphics::DrawMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix)
{
	auto scenePtr = SceneManager::getInstance()->GetMainScene();
	if (scenePtr == nullptr)
	{
		return;
	}
	auto gameObject = SceneUtility::CreateMeshGameObject(mesh, material);
	auto transform = gameObject->GetComponent<Transform>();
	transform->SetMatrix(modelMatrix);
	scenePtr->AddObject(gameObject);
}

void Graphics::DrawMeshNow(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix)
{
	ComponentStateMachine componentStateMachine;
	auto scenePtr = SceneManager::getInstance()->GetMainScene();
	if (scenePtr == nullptr)
	{
		return;
	}
	auto gameObject = SceneUtility::CreateMeshGameObject(mesh, material);
	auto transform = gameObject->GetComponent<Transform>();
	auto renderer = gameObject->GetComponent<Renderer>();
	transform->SetMatrix(modelMatrix);
	scenePtr->AddObject(gameObject);

	componentStateMachine.Setup(renderer);
	while (componentStateMachine.GetComponentLifeStyle() != ComponentLifeCycle::Running)
	{
		componentStateMachine.Tick();
	}
	DrawRequestedMesh(mesh, material, camera, modelMatrix);
}
void Graphics::DrawRequestedMesh(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, std::shared_ptr<Camera>& camera, const glm::mat4x4& modelMatrix)
{
	auto resourceIdentifier = ResourceManager::getInstance()->GetMeshResource(mesh->GetInstanceId());
	if (!resourceIdentifier.IsValid())
	{
		return;
	}
	if (resourceIdentifier.IsValid())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resourceIdentifier.GetEBO());

		glBindVertexArray(resourceIdentifier.GetVAO());

		glBindBuffer(GL_ARRAY_BUFFER, resourceIdentifier.GetVBO());

		auto mainTexProperty = material->GetMainTex();
		auto shader = material->GetShader();
		GLuint program = shader->GetShaderProgram();

		if (program > 0 && glIsProgram(program))
		{
			auto lightProperties = SceneManager::getInstance()->GetAffectedLights(camera);
			glUseProgram(program);
			Shader::SetGlobalMatrix(ShaderPropertyNames::ModelMatrix, modelMatrix);
			Shader::Upload(ConstantBufferType::PerPass);
			ShaderUtils::bindBlockForProgram(*shader.get());
			//shader->SetUniformBlock(ShaderPropertyNames::Lights)
			//shader->SetVector(ShaderPropertyNames::Lights, &lightProperties);

			Texture* mainTex = mainTexProperty == nullptr ? nullptr : ((MaterialTextureProperty*)mainTexProperty.get())->GetTexture().get();
			if (mainTex != nullptr)
			{
				auto textureIdentifier = ResourceManager::getInstance()->GetTextureResource(mainTex->GetInstanceId());
				auto target = Texture::TextureType2TextureTarget(textureIdentifier.GetTextureType());
				if (target != GL_NONE)
				{
					glBindTextureUnit(0, textureIdentifier.GetTextureHandle());
				}
			}
		}
		glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_SHORT, NULL);
	}
}