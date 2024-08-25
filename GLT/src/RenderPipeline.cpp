#include "RenderPipeline.h"
#include <RenderTexture.h>
void RenderPipeline::init() {
	m_renderContext.init();
	ShaderUtils::loadAllShader(m_renderContext);
}
void RenderPipeline::uninit() {
	ShaderUtils::unloadAllShader(m_renderContext);
	m_renderContext.uninit();
}
void RenderPipeline::render() {
	updateSceneProperties4Render();

	// do someting
	updatePerFrameConstantBuffer();

	auto& rtManagementCentre = RenderResourceManager::getInstance()->getRenderTargetManagementCentre();
	auto instanceIds = rtManagementCentre.getAllObjectInstanceIds();
	if (instanceIds.size() > 0)
	{
		auto rtIdentifier = RenderResourceManager::getInstance()->getRenderTargetResource(instanceIds[0]);
		m_cmd.setRenderTarget(rtIdentifier);
		m_cmd.clearColor(0.0f, 0.0f, 0.0f, 0.0f);
		m_renderContext.scheduleCommandBuffer(m_cmd);
		m_cmd.clear();
		m_renderContext.submit();
	}
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
		m_cmd.setViewport(viewPortRect.x, viewPortRect.y, viewPortRect.z, viewPortRect.w);
		for (auto& renderObject : scene->getObjectList())
		{
			auto renderer = renderObject->getComponent<Renderer>();
			if (renderer != nullptr)
			{
				renderPerObject(*renderer, mainCamera);
			}
		}
	}
	m_renderContext.blitToWindow();

	postUpdate();
}
void RenderPipeline::postUpdate()
{
	RenderResourceManager::getInstance()->getMeshManagementCentre().onSubmit();
	RenderResourceManager::getInstance()->getTextureManagementCentre().onSubmit();
}

void RenderPipeline::updateSceneProperties4Render()
{
	m_allScenes = SceneManager::getInstance()->getAllScenes(false);

	// É¾³ý
	clearExpiredObjects(&RenderResourceManager::getInstance()->getMeshManagementCentre(), ResourceType::Mesh);
	clearExpiredObjects(&RenderResourceManager::getInstance()->getTextureManagementCentre(), ResourceType::Texture);
	clearExpiredObjects(&RenderResourceManager::getInstance()->getRenderTargetManagementCentre(), ResourceType::RenderTarget);

	// Ìí¼Ó
	appendNewObjects(&RenderResourceManager::getInstance()->getMeshManagementCentre(), ResourceType::Mesh);
	appendNewObjects(&RenderResourceManager::getInstance()->getTextureManagementCentre(), ResourceType::Texture);
	appendNewObjects(&RenderResourceManager::getInstance()->getRenderTargetManagementCentre(), ResourceType::RenderTarget);
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
	RenderResourceManager::getInstance()->uploadConstantBufferResource(ConstantBufferType::PerFrame);
}

void RenderPipeline::updatePerCameraConstantBuffer(std::shared_ptr<Camera>& camera)
{
	updateLightProperties(camera);
	Shader::setGlobalMatrix(ShaderPropertyNames::ViewMatrix, camera->getViewMatrix());
	Shader::setGlobalMatrix(ShaderPropertyNames::ProjectMatrix, camera->getProjectMatrix());
	auto eyePosition = camera->getTransform()->GetPosition();
	Shader::setGlobalVector(ShaderPropertyNames::EyePosition, glm::vec4(eyePosition.x, eyePosition.y, eyePosition.z, 1.0f));
	RenderResourceManager::getInstance()->uploadConstantBufferResource(ConstantBufferType::PerCamera);
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

	drawMesh(renderObject.GetMesh().get(), renderObject.GetMaterial().get(), renderObject.GetGameObject()->getTransform()->GetMatrix());
}

void RenderPipeline::drawMesh(Mesh* mesh, Material* material, glm::mat4 modelMatrix)
{
	auto* resourceIdentifier = RenderResourceManager::getInstance()->getMeshResource(mesh->getInstanceId());
	if (resourceIdentifier == nullptr || !resourceIdentifier->isValid())
	{
		return;
	}
	m_cmd.drawMesh(mesh, material, modelMatrix);
	m_renderContext.scheduleCommandBuffer(m_cmd);
	m_renderContext.submit();
	m_cmd.clear();
}