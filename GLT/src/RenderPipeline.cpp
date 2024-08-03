#include "RenderPipeline.h"
void RenderPipeline::Init() {
	m_renderContext.Init();
	ShaderUtils::LoadAllShader(m_renderContext);
}
void RenderPipeline::UnInit() {

}
void RenderPipeline::ClearExpiredMeshBuffers(std::vector<std::shared_ptr<Scene>>& allScenes)
{
	// É¾³ý
	for (int i = 0;i < allScenes.size();++i)
	{
		ClearExpiredObjects(&ResourceManager::getInstance()->GetMeshManagementCentre(), ResourceType::Mesh);
	}
}
void RenderPipeline::AppendNewMeshBuffers(std::vector<std::shared_ptr<Scene>>& allScenes)
{
	for (int i = 0;i < allScenes.size();++i)
	{
		AppendNewObjects(&ResourceManager::getInstance()->GetMeshManagementCentre(), ResourceType::Mesh);
	}
}

void RenderPipeline::ClearExpiredTextures(std::vector<std::shared_ptr<Scene>>& allScenes)
{
	for (int i = 0;i < allScenes.size();++i)
	{
		ClearExpiredObjects(&ResourceManager::getInstance()->GetTextureManagementCentre(), ResourceType::Texture);
	}
}
void RenderPipeline::AppendNewTextures(std::vector<std::shared_ptr<Scene>>& allScenes)
{
	for (int i = 0;i < allScenes.size();++i)
	{
		AppendNewObjects(&ResourceManager::getInstance()->GetTextureManagementCentre(), ResourceType::Texture);
	}
}

void RenderPipeline::Render() {
	UpdateSceneProperties4Render();

	// do someting
	UpdatePerFrameConstantBuffer();
	for (const auto& scene : m_allScenes)
	{
		auto mainCamera = scene->GetMainCamera();
		if (mainCamera == nullptr)
		{
			continue;
		}
		UpdatePerCameraConstantBuffer(mainCamera);

		auto viewPort = mainCamera->GetViewPort();
		auto windowSize = Window::getInstance()->GetSize();
		auto viewPortRect = glm::ivec4(viewPort.x * windowSize.x, viewPort.y * windowSize.y, viewPort.z * windowSize.x, viewPort.w * windowSize.y);
		glViewport(viewPortRect.x, viewPortRect.y, viewPortRect.z, viewPortRect.w);
		for (auto& renderObject : scene->GetObjectList())
		{
			auto renderer = renderObject->GetComponent<Renderer>();
			if (renderer != nullptr)
			{
				RenderPerObject(*renderer, mainCamera);
			}
		}
	}
	PostUpdate();
}
void RenderPipeline::PostUpdate()
{
	ResourceManager::getInstance()->GetMeshManagementCentre().OnSubmit();
	ResourceManager::getInstance()->GetTextureManagementCentre().OnSubmit();
}

void RenderPipeline::UpdateSceneProperties4Render()
{
	m_allScenes = SceneManager::getInstance()->GetAllScenes(false);

	// É¾³ý
	ClearExpiredMeshBuffers(m_allScenes);
	ClearExpiredTextures(m_allScenes);

	// Ìí¼Ó
	AppendNewMeshBuffers(m_allScenes);
	AppendNewTextures(m_allScenes);
}

void RenderPipeline::UpdateLightProperties(std::shared_ptr<Camera>& camera)
{
	m_lightProperties = SceneManager::getInstance()->GetAffectedLights(camera);
	if (m_lightProperties.size() > 0)
	{
		auto lightProperties = m_lightProperties[0];
		Shader::SetGlobalVector(ShaderPropertyNames::MainLightData_Ambient, glm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
		Shader::SetGlobalVector(ShaderPropertyNames::MainLightData_Color, lightProperties.color);
		Shader::SetGlobalVector(ShaderPropertyNames::MainLightData_Position, glm::vec4(lightProperties.position.x, lightProperties.position.y, lightProperties.position.z, 1.0f));
		Shader::SetGlobalVector(ShaderPropertyNames::MainLightData_ConeDirection, glm::vec4(lightProperties.coneDirection.x, lightProperties.coneDirection.y, lightProperties.coneDirection.z, 1.0f));
		Shader::SetGlobalFloat(ShaderPropertyNames::MainLightData_EndDistance, -1);
		Shader::SetGlobalVector(ShaderPropertyNames::MainLightData_Attenuations, glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
	}
	for (int i = 0;i < m_lightProperties.size() - 1;++i)
	{
		auto lightProperties = m_lightProperties[i + 1];
		Shader::SetGlobalVector(ShaderPropertyNames::GetShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Ambient, i).c_str(), glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
		Shader::SetGlobalVector(ShaderPropertyNames::GetShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Color, i).c_str(), lightProperties.color);
		Shader::SetGlobalVector(ShaderPropertyNames::GetShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Position, i).c_str(), glm::vec4(lightProperties.position.x, lightProperties.position.y, lightProperties.position.z, 1.0f));
		Shader::SetGlobalVector(ShaderPropertyNames::GetShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_ConeDirection, i).c_str(), glm::vec4(lightProperties.coneDirection.x, lightProperties.coneDirection.y, lightProperties.coneDirection.z, 1.0f));
		Shader::SetGlobalFloat(ShaderPropertyNames::GetShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_EndDistance, i).c_str(), 100.0f);
		Shader::SetGlobalVector(ShaderPropertyNames::GetShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Attenuations, i).c_str(), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
		/*ss.str("");
		ss << ShaderPropertyNames::AddtionalLightData_Format << "[" << i << "]." << ShaderPropertyNames::AddtionalLightData_SpotCosCutOff_Format;
		Shader::SetGlobalFloat(ss.str().c_str(), lightProperties.spotCosCutoff);*/
	}
}

void RenderPipeline::UpdatePerFrameConstantBuffer()
{
	// do something
	Shader::Upload(ConstantBufferType::PerFrame);
}

void RenderPipeline::UpdatePerCameraConstantBuffer(std::shared_ptr<Camera>& camera)
{
	UpdateLightProperties(camera);
	Shader::SetGlobalMatrix(ShaderPropertyNames::ViewMatrix, camera->GetViewMatrix());
	Shader::SetGlobalMatrix(ShaderPropertyNames::ProjectMatrix, camera->GetProjectMatrix());
	auto eyePosition = camera->GetTransform()->GetPosition();
	Shader::SetGlobalVector(ShaderPropertyNames::EyePosition, glm::vec4(eyePosition.x, eyePosition.y, eyePosition.z, 1.0f));
	Shader::Upload(ConstantBufferType::PerCamera);
}

void RenderPipeline::RenderPerObject(Renderer& renderObject, std::shared_ptr<Camera>& camera)
{
	auto mesh = renderObject.GetMesh();
	if (mesh == nullptr)
	{
		return;
	}
	auto go = renderObject.GetGameObject();
	if (go == nullptr)
	{
		return;
	}
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDepthRange(0, 1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	Graphics::DrawRequestedMesh(mesh, renderObject.GetMaterial(), camera, go->GetTransform()->GetMatrix());
}
