#include "RenderPipeline.h"
void RenderPipeline::init() {
	m_globalCameraPassList.push_back(new ShadowMapPass());
	//m_perCameraPassList.push_back(new DrawOpaquePass());
	m_renderContext.init();
	ShaderUtils::loadAllShader(m_renderContext);
}
void RenderPipeline::uninit() {
	ShaderUtils::unloadAllShader(m_renderContext);
	m_renderContext.uninit();

	for (const auto& pass : m_perCameraPassList)
	{
		pass->destroy();
		delete pass;
	}
	for (const auto& pass : m_globalCameraPassList)
	{
		pass->destroy();
		delete pass;
	}
	m_perCameraPassList.clear();
	m_globalCameraPassList.clear();
}
void RenderPipeline::render() {

	LogicResourceManager::getInstance()->tick();

	updatePerFrameConstantBuffer();

	m_allScenes = SceneManager::getInstance()->getAllScenes(false);

	m_renderData.m_cameraDatas.clear();

	// 收集一些绘制需要的数据
	for (const auto& scene : m_allScenes)
	{
		auto mainCamera = scene->getMainCamera();
		if (mainCamera == nullptr)
		{
			continue;
		}
		// 相机数据
		auto cameraData = CameraData();
		cameraData.m_worldPos = static_cast<GameObject*>(mainCamera->getGameObject())->getTransform()->getPosition();
		cameraData.m_viewMatrix = mainCamera->getViewMatrix();
		cameraData.m_projectionMatrix = mainCamera->getProjectMatrix();
		cameraData.m_viewport = mainCamera->getViewPort();;
		m_renderData.m_cameraDatas.push_back(cameraData);
		// 光照数据
		m_renderData.m_lightDatas = SceneManager::getInstance()->getAffectedLights(mainCamera);
	}

	// 逐相机调用Pass执行
	for (const auto& pass : m_globalCameraPassList)
	{
		if (!pass->IsPrepared())
		{
			pass->setup(&m_renderContext, &m_renderData);
			pass->prepare();
		}
		pass->execute();
	}

	for (int i = 0; i < m_renderData.m_cameraDatas.size(); ++i)
	{
		m_renderData.m_curRenderingCameraIndex = i;
		auto cameraData = m_renderData.m_cameraDatas[i];
		// 更新逐相机常量缓冲区
		updatePerCameraConstantBuffer(cameraData);

		// 设置Viewport
		auto viewPort = cameraData.m_viewport;
		auto windowSize = Window::getInstance()->getSize();
		auto viewPortRect = glm::ivec4(viewPort.x * windowSize.x, viewPort.y * windowSize.y, viewPort.z * windowSize.x, viewPort.w * windowSize.y);
		m_cmd.setViewport(viewPortRect.x, viewPortRect.y, viewPortRect.z, viewPortRect.w);
		m_renderContext.scheduleCommandBuffer(m_cmd);
		m_cmd.clear();

		// 逐相机调用Pass执行
		for (const auto& pass : m_perCameraPassList)
		{
			if (!pass->IsPrepared())
			{
				pass->setup(&m_renderContext, &m_renderData);
				pass->prepare();
			}
			pass->execute();
		}
	}
	// 绘制完毕后,从绘制区FBO Blit到窗口区FBO
	m_renderContext.blitToWindow();

	postUpdate();
}
void RenderPipeline::postUpdate()
{
}

void RenderPipeline::updateLightProperties()
{
	auto& lightDatas = m_renderData.m_lightDatas;
	if (lightDatas.size() > 0)
	{
		auto lightProperties = lightDatas[0];
		Shader::setGlobalVector(ShaderPropertyNames::MainLightData_Ambient, glm::vec4(0.2f, 0.2f, 0.2f, 0.0f));
		Shader::setGlobalVector(ShaderPropertyNames::MainLightData_Color, lightProperties.color);
		Shader::setGlobalVector(ShaderPropertyNames::MainLightData_Position, glm::vec4(lightProperties.position.x, lightProperties.position.y, lightProperties.position.z, 1.0f));
		Shader::setGlobalVector(ShaderPropertyNames::MainLightData_ConeDirection, glm::vec4(lightProperties.direction.x, lightProperties.direction.y, lightProperties.direction.z, 1.0f));
		Shader::setGlobalFloat(ShaderPropertyNames::MainLightData_EndDistance, -1);
		Shader::setGlobalVector(ShaderPropertyNames::MainLightData_Attenuations, glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
	}
	for (int i = 0; i < lightDatas.size() - 1; ++i)
	{
		auto lightProperties = lightDatas[i + 1];
		Shader::setGlobalVector(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Ambient, i).c_str(), glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
		Shader::setGlobalVector(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Color, i).c_str(), lightProperties.color);
		Shader::setGlobalVector(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Position, i).c_str(), glm::vec4(lightProperties.position.x, lightProperties.position.y, lightProperties.position.z, 1.0f));
		Shader::setGlobalVector(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_ConeDirection, i).c_str(), glm::vec4(lightProperties.direction.x, lightProperties.direction.y, lightProperties.direction.z, 1.0f));
		Shader::setGlobalFloat(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_EndDistance, i).c_str(), 100.0f);
		Shader::setGlobalVector(ShaderPropertyNames::getShaderArrayPropertyName(ShaderPropertyNames::AddtionalLightData, ShaderPropertyNames::AddtionalLightData_Attenuations, i).c_str(), glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
		/*ss.str("");
		ss << ShaderPropertyNames::AddtionalLightData_Format << "[" << i << "]." << ShaderPropertyNames::AddtionalLightData_SpotCosCutOff_Format;
		Shader::setGlobalFloat(ss.str().c_str(), lightProperties.spotCosCutoff);*/
	}
}

void RenderPipeline::updatePerFrameConstantBuffer()
{
	m_renderContext.updateConstantBufferResources(ConstantBufferType::PerFrame);
}

void RenderPipeline::updatePerCameraConstantBuffer(CameraData& cameraData)
{
	updateLightProperties();
	m_cmd.setViewMatrix(cameraData.m_viewMatrix);
	m_cmd.setProjectionMatrix(cameraData.m_projectionMatrix);
	m_renderContext.scheduleCommandBuffer(m_cmd);
	m_cmd.clear();
	m_renderContext.submit();
	auto eyePosition = cameraData.m_worldPos;
	Shader::setGlobalVector(ShaderPropertyNames::EyePosition, glm::vec4(eyePosition.x, eyePosition.y, eyePosition.z, 1.0f));
	m_renderContext.updateConstantBufferResources(ConstantBufferType::PerCamera);
}

void RenderPipeline::drawMesh(Mesh* mesh, Material* material, glm::mat4 modelMatrix)
{
	auto resourceIdentifier = static_cast<MeshResourceIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Mesh, mesh->getInstanceId()));
	if (resourceIdentifier == nullptr || !resourceIdentifier->isValid())
	{
		return;
	}
	m_cmd.drawMesh(mesh, material, modelMatrix);
	m_renderContext.scheduleCommandBuffer(m_cmd);
	m_renderContext.submit();
	m_cmd.clear();
}