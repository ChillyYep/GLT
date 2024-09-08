#include "CommandBuffer.h"

void CommandBuffer::setRenderTarget(RenderTargetIdentifier* renderTargetIdentifier)
{
	assert(renderTargetIdentifier != nullptr);
	RenderCommand command = RenderCommand();
	SetRenderTargetParam* setRenderTargetParam = RenderCommandParamFactory::getInstance()->createParam<SetRenderTargetParam>();
	setRenderTargetParam->identifier = renderTargetIdentifier;

	command.commandType = RenderCommandType::SetRenderTarget;
	command.param = setRenderTargetParam;
	m_cmdList.push_back(command);
}

void CommandBuffer::clearColor(float r, float g, float b, float a)
{
	RenderCommand command = RenderCommand();
	ClearColorRenderParam* clearColorParam = RenderCommandParamFactory::getInstance()->createParam<ClearColorRenderParam>();
	clearColorParam->r = r;
	clearColorParam->g = g;
	clearColorParam->b = b;
	clearColorParam->a = a;

	command.commandType = RenderCommandType::ClearColor;
	command.param = clearColorParam;
	m_cmdList.push_back(command);
}

void CommandBuffer::clear()
{
	m_cmdList.clear();
}

void CommandBuffer::drawMesh(Mesh* mesh, Material* material, glm::mat4 modelMatrix)
{
	auto meshResourceIdentifier = static_cast<MeshResourceIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Mesh, mesh->getInstanceId()));
	if (meshResourceIdentifier == nullptr)
	{
		return;
	}
	RenderCommand command = RenderCommand();
	DrawMeshParam* drawMeshParam = RenderCommandParamFactory::getInstance()->createParam<DrawMeshParam>();
	drawMeshParam->m_meshPtr = mesh;
	drawMeshParam->m_materialPtr = material;
	drawMeshParam->m_modelMatrix = modelMatrix;
	drawMeshParam->m_meshResourceIdentifier = static_cast<MeshResourceIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Mesh, mesh->getInstanceId()));
	if (material != nullptr)
	{
		std::vector<Texture*> textures = material->getAllTextures();
		for (const auto& texPtr : textures)
		{
			auto texIdentifierPtr = RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Texture, texPtr->getInstanceId());
			drawMeshParam->m_textureResources.push_back(static_cast<TextureResourceIdentifier*>(texIdentifierPtr));
		}
	}
	command.commandType = RenderCommandType::DrawMesh;
	command.param = drawMeshParam;
	m_cmdList.push_back(command);
}

void CommandBuffer::drawRenderer(Renderer* renderer)
{
	auto meshResourceIdentifier = static_cast<MeshResourceIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Mesh, (renderer->getMesh()->getInstanceId())));
	if (meshResourceIdentifier == nullptr)
	{
		return;
	}
	RenderCommand command = RenderCommand();
	DrawRendererParam* drawRendererParam = RenderCommandParamFactory::getInstance()->createParam<DrawRendererParam>();
	drawRendererParam->m_rendererPtr = renderer;
	drawRendererParam->m_meshResourceIdentifier = meshResourceIdentifier;

	if (renderer->getMaterial() != nullptr)
	{
		std::vector<Texture*> textures = renderer->getMaterial()->getAllTextures();
		for (const auto& texPtr : textures)
		{
			auto texIdentifierPtr = RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Texture, texPtr->getInstanceId());
			drawRendererParam->m_textureResources.push_back(static_cast<TextureResourceIdentifier*>(texIdentifierPtr));
		}
	}
	command.commandType = RenderCommandType::DrawRenderer;
	command.param = drawRendererParam;

	m_cmdList.push_back(command);
}

void CommandBuffer::setViewport(int x, int y, int width, int height)
{
	RenderCommand command = RenderCommand();
	SetViewPortParam* setViewPortParam = RenderCommandParamFactory::getInstance()->createParam<SetViewPortParam>();
	setViewPortParam->m_x = x;
	setViewPortParam->m_y = y;
	setViewPortParam->m_width = width;
	setViewPortParam->m_height = height;
	command.commandType = RenderCommandType::SetViewPort;
	command.param = setViewPortParam;
	m_cmdList.push_back(command);
}