#include "RenderPipeline.h"
void RenderPipeline::init() {
	m_renderContext.init();
	ShaderUtils::loadAllShader(m_renderContext);
}
void RenderPipeline::uninit() {

}
void RenderPipeline::clearExpiredMeshBuffers(std::vector<std::shared_ptr<Scene>>& allScenes)
{
	// É¾³ý
	for (int i = 0;i < allScenes.size();++i)
	{
		clearExpiredObjects(&ResourceManager::getInstance()->getMeshManagementCentre(), ResourceType::Mesh);
	}
}
void RenderPipeline::appendNewMeshBuffers(std::vector<std::shared_ptr<Scene>>& allScenes)
{
	for (int i = 0;i < allScenes.size();++i)
	{
		appendNewObjects(&ResourceManager::getInstance()->getMeshManagementCentre(), ResourceType::Mesh);
	}
}

void RenderPipeline::clearExpiredTextures(std::vector<std::shared_ptr<Scene>>& allScenes)
{
	for (int i = 0;i < allScenes.size();++i)
	{
		clearExpiredObjects(&ResourceManager::getInstance()->getTextureManagementCentre(), ResourceType::Texture);
	}
}
void RenderPipeline::appendNewTextures(std::vector<std::shared_ptr<Scene>>& allScenes)
{
	for (int i = 0;i < allScenes.size();++i)
	{
		appendNewObjects(&ResourceManager::getInstance()->getTextureManagementCentre(), ResourceType::Texture);
	}
}

void RenderPipeline::render() {
	updateSceneProperties4Render();

	// do someting
	updatePerFrameConstantBuffer();
	for (const auto& scene : m_allScenes)
	{
		auto mainCamera = scene->getMainCamera();
		if (mainCamera == nullptr)
		{
			continue;
		}
		updatePerCameraConstantBuffer(mainCamera);

		auto viewPort = mainCamera->GetViewPort();
		auto windowSize = Window::getInstance()->getSize();
		auto viewPortRect = glm::ivec4(viewPort.x * windowSize.x, viewPort.y * windowSize.y, viewPort.z * windowSize.x, viewPort.w * windowSize.y);
		glViewport(viewPortRect.x, viewPortRect.y, viewPortRect.z, viewPortRect.w);
		for (auto& renderObject : scene->getObjectList())
		{
			auto renderer = renderObject->getComponent<Renderer>();
			if (renderer != nullptr)
			{
				renderPerObject(*renderer, mainCamera);
			}
		}
	}
	postUpdate();
}
void RenderPipeline::postUpdate()
{
	ResourceManager::getInstance()->getMeshManagementCentre().onSubmit();
	ResourceManager::getInstance()->getTextureManagementCentre().onSubmit();
}

void RenderPipeline::updateSceneProperties4Render()
{
	m_allScenes = SceneManager::getInstance()->getAllScenes(false);

	// É¾³ý
	clearExpiredMeshBuffers(m_allScenes);
	clearExpiredTextures(m_allScenes);

	// Ìí¼Ó
	appendNewMeshBuffers(m_allScenes);
	appendNewTextures(m_allScenes);
}

void RenderPipeline::updateLightProperties(std::shared_ptr<Camera>& camera)
{
	m_lightProperties = SceneManager::getInstance()->getAffectedLights(camera);
	if (m_lightProperties.size() > 0)
	{
		auto lightProperties = m_lightProperties[0];
		Shader::setGlobalVector(ShaderPropertyNames::MainLightData_Ambient, glm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
		Shader::setGlobalVector(ShaderPropertyNames::MainLightData_Color, lightProperties.color);
		Shader::setGlobalVector(ShaderPropertyNames::MainLightData_Position, glm::vec4(lightProperties.position.x, lightProperties.position.y, lightProperties.position.z, 1.0f));
		Shader::setGlobalVector(ShaderPropertyNames::MainLightData_ConeDirection, glm::vec4(lightProperties.direction.x, lightProperties.direction.y, lightProperties.direction.z, 1.0f));
		Shader::setGlobalFloat(ShaderPropertyNames::MainLightData_EndDistance, -1);
		Shader::setGlobalVector(ShaderPropertyNames::MainLightData_Attenuations, glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
	}
	for (int i = 0;i < m_lightProperties.size() - 1;++i)
	{
		auto lightProperties = m_lightProperties[i + 1];
		Shader::setGlobalVector(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Ambient, i).c_str(), glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
		Shader::setGlobalVector(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Color, i).c_str(), lightProperties.color);
		Shader::setGlobalVector(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Position, i).c_str(), glm::vec4(lightProperties.position.x, lightProperties.position.y, lightProperties.position.z, 1.0f));
		Shader::setGlobalVector(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_ConeDirection, i).c_str(), glm::vec4(lightProperties.direction.x, lightProperties.direction.y, lightProperties.direction.z, 1.0f));
		Shader::setGlobalFloat(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_EndDistance, i).c_str(), 100.0f);
		Shader::setGlobalVector(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Attenuations, i).c_str(), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
		/*ss.str("");
		ss << ShaderPropertyNames::AddtionalLightData_Format << "[" << i << "]." << ShaderPropertyNames::AddtionalLightData_SpotCosCutOff_Format;
		Shader::SetGlobalFloat(ss.str().c_str(), lightProperties.spotCosCutoff);*/
	}
}

void RenderPipeline::updatePerFrameConstantBuffer()
{
	// do something
	Shader::upload(ConstantBufferType::PerFrame);
}

void RenderPipeline::updatePerCameraConstantBuffer(std::shared_ptr<Camera>& camera)
{
	updateLightProperties(camera);
	Shader::setGlobalMatrix(ShaderPropertyNames::ViewMatrix, camera->getViewMatrix());
	Shader::setGlobalMatrix(ShaderPropertyNames::ProjectMatrix, camera->getProjectMatrix());
	auto eyePosition = camera->getTransform()->GetPosition();
	Shader::setGlobalVector(ShaderPropertyNames::EyePosition, glm::vec4(eyePosition.x, eyePosition.y, eyePosition.z, 1.0f));
	Shader::upload(ConstantBufferType::PerCamera);
}

void RenderPipeline::renderPerObject(Renderer& renderObject, std::shared_ptr<Camera>& camera)
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
	Graphics::drawRequestedMesh(mesh, renderObject.GetMaterial(), camera, go->getTransform()->GetMatrix());
}
