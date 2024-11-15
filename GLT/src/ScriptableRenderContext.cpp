#include "ScriptableRenderContext.h"

void ScriptableRenderContext::init()
{
	m_device = new GLDevice();
	RenderResourceManagement::getInstance()->setup(m_device);
	RenderResourceManagement::getInstance()->regist();
}

void ScriptableRenderContext::uninit()
{
	RenderResourceManagement::getInstance()->unregist();
	if (m_device != nullptr)
	{
		delete m_device;
		m_device = nullptr;
	}
}
void ScriptableRenderContext::drawRenderers(FilterSettings& filterSetting, DrawSettings& drawSetting, Material* replacedMaterial)
{
	m_filterSetting = filterSetting;

	m_drawSetting = drawSetting;

	m_renderers = SceneManager::getInstance()->filterRenderers(filterSetting.m_renderType);

	std::vector<RendererSortStructure> simpleStructure(m_renderers.size());

	for (int i = 0; i < m_renderers.size(); ++i)
	{
		auto pos = m_renderers[i]->getWorldBound().m_center;
		simpleStructure[i].m_target = m_renderers[i];
		simpleStructure[i].m_sortOrder = glm::length(pos - m_drawSetting.m_cameraPos);
	}

	if (drawSetting.m_sortType == SortType::Near2Far)
	{
		std::sort(simpleStructure.begin(), simpleStructure.end(), sortNear2FarCompare);
	}
	else if (drawSetting.m_sortType == SortType::Far2Near)
	{
		std::sort(simpleStructure.begin(), simpleStructure.end(), sortFar2NearCompare);
	}
	for (int i = 0; i < m_renderers.size(); ++i)
	{
		m_renderers[i] = simpleStructure[i].m_target;
		m_cmdList.drawRenderer(m_renderers[i], replacedMaterial);
	}
	scheduleCommandBuffer(m_cmdList);
	m_cmdList.clear();
	submit();
}

void ScriptableRenderContext::scheduleCommandBuffer(CommandBuffer commandBuffer)
{
	auto& commands = commandBuffer.getAllCmdList();
	for (int i = 0; i < commands.size(); ++i)
	{
		m_commands.push_back(commands[i]);
	}
}

void ScriptableRenderContext::submit()
{
	for (int i = 0; i < m_commands.size(); ++i)
	{
		executeCommand(m_commands[i]);
	}
	m_commands.clear();
}

void ScriptableRenderContext::blitCurrentRTToWindow()
{
	m_device->blitCurrentRTToWindow();
}

void ScriptableRenderContext::executeCommand(RenderCommand& command)
{
	auto commandType = command.commandType;
	auto commandParam = command.param;
	switch (commandType)
	{
	case RenderCommandType::SetViewPort:
	{
		auto setViewPortParam = static_cast<SetViewPortParam*>(commandParam);
		m_device->setViewport(setViewPortParam->m_x, setViewPortParam->m_y, setViewPortParam->m_width, setViewPortParam->m_height);
		break;
	}
	case RenderCommandType::SetRenderTarget:
	{
		auto setRenderTargetParam = static_cast<SetRenderTargetParam*>(commandParam);
		m_device->activate(setRenderTargetParam->identifier);
		break;
	}
	case RenderCommandType::ClearColor:
	{
		auto clearColorParam = static_cast<ClearColorRenderParam*>(commandParam);
		m_device->clearColor(clearColorParam->r, clearColorParam->g, clearColorParam->b, clearColorParam->a);
		break;
	}
	case RenderCommandType::DrawMesh:
	{
		auto drawMeshParam = static_cast<DrawMeshParam*>(commandParam);
		auto meshResourceIdentifier = static_cast<MeshResourceIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Mesh, drawMeshParam->m_meshPtr->getInstanceId()));
		if (meshResourceIdentifier != nullptr)
		{
			std::vector<TextureResourceIdentifier*> textureResources;
			if (drawMeshParam->m_materialPtr != nullptr)
			{
				std::vector<Texture*> textures = drawMeshParam->m_materialPtr->getAllTextures();
				for (const auto& texPtr : textures)
				{
					auto texIdentifierPtr = RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Texture, texPtr->getInstanceId());
					textureResources.push_back(static_cast<TextureResourceIdentifier*>(texIdentifierPtr));
				}
				setupPSO(meshResourceIdentifier, drawMeshParam->m_modelMatrix, drawMeshParam->m_materialPtr, textureResources);
				m_device->draw(m_pso);
			}
		}
		break;
	}
	case RenderCommandType::DrawRenderer:
	{
		auto drawRendererParam = static_cast<DrawRendererParam*>(commandParam);
		auto renderer = drawRendererParam->m_rendererPtr;
		auto meshCount = (int)renderer->getMeshCount();
		auto materials = renderer->getMaterials();
		auto meshes = renderer->getMeshes();
		auto transform = static_cast<GameObject*>(renderer->getGameObject())->getTransform();
		for (int i = 0; i < meshCount; ++i)
		{
			auto meshResourceIdentifier = static_cast<MeshResourceIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Mesh, (meshes[i]->getInstanceId())));
			if (meshResourceIdentifier != nullptr)
			{
				auto renderMat = drawRendererParam->m_replacedMaterial != nullptr ? drawRendererParam->m_replacedMaterial : materials[i].get();
				std::vector<TextureResourceIdentifier*> textureResources;
				if (renderMat != nullptr)
				{
					std::vector<Texture*> textures = renderMat->getAllTextures();
					for (const auto& texPtr : textures)
					{
						auto texIdentifierPtr = RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Texture, texPtr->getInstanceId());
						textureResources.push_back(static_cast<TextureResourceIdentifier*>(texIdentifierPtr));
					}
					setupPSO(meshResourceIdentifier, transform->getModelMatrix(), renderMat, textureResources);
					m_device->draw(m_pso);
				}
			}
		}

		break;
	}
	case RenderCommandType::SetViewMatrix:
	{
		auto setViewMatrixParam = static_cast<SetViewMatrixParam*>(commandParam);
		m_device->setViewMatrix(setViewMatrixParam->m_viewMatrix);
		break;
	}
	case RenderCommandType::SetProjectionMatrix:
	{
		auto setProjectionMatrixParam = static_cast<SetProjectionMatrixParam*>(commandParam);
		m_device->setProjectionMatrix(setProjectionMatrixParam->m_projectionMatrix);
		m_device->setProjectionParams(setProjectionMatrixParam->m_projectionParams);
		m_device->setScreenParams(setProjectionMatrixParam->m_screenParams);
		break;
	}
	case RenderCommandType::SetGlobalTextureResource:
	{
		auto setGlobalTextureResource = static_cast<SetGlobalTextureResource*>(commandParam);

		if (setGlobalTextureResource->m_globalTextureResource != nullptr)
		{
			addGlobalTextureResource(setGlobalTextureResource->m_shaderPropertyName, setGlobalTextureResource->m_globalTextureResource);
		}
		break;
	}
	default:
		break;
	}

	m_device->release(command);
}