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
void CommandBuffer::setGlobalTextureResource(std::string propertyName, TextureResourceIdentifier* textureResource, unsigned int registerIndex)
{
	assert(textureResource != nullptr);
	RenderCommand command = RenderCommand();
	SetGlobalTextureResource* setGlobalTextureResourceParam = RenderCommandParamFactory::getInstance()->createParam<SetGlobalTextureResource>();
	setGlobalTextureResourceParam->m_globalTextureResource = textureResource;
	setGlobalTextureResourceParam->m_shaderPropertyName = propertyName;
	setGlobalTextureResourceParam->m_registerIndex = registerIndex;

	command.commandType = RenderCommandType::SetGlobalTextureResource;
	command.param = setGlobalTextureResourceParam;
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

void CommandBuffer::drawMesh(SubMesh* mesh, Material* material, glm::mat4 modelMatrix)
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
	command.commandType = RenderCommandType::DrawMesh;
	command.param = drawMeshParam;
	m_cmdList.push_back(command);
}

void CommandBuffer::drawRenderer(Renderer* renderer, Material* replacedMaterial)
{
	RenderCommand command = RenderCommand();
	DrawRendererParam* drawRendererParam = RenderCommandParamFactory::getInstance()->createParam<DrawRendererParam>();
	drawRendererParam->m_rendererPtr = renderer;
	drawRendererParam->m_replacedMaterial = replacedMaterial;

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

void CommandBuffer::setProjectionMatrix(glm::mat4 projection, glm::vec4 projectionParams, glm::vec4 screenParams)
{
	RenderCommand command = RenderCommand();
	SetProjectionMatrixParam* setProjectionMatrixParam = RenderCommandParamFactory::getInstance()->createParam<SetProjectionMatrixParam>();
	setProjectionMatrixParam->m_projectionMatrix = projection;
	setProjectionMatrixParam->m_projectionParams = projectionParams;
	setProjectionMatrixParam->m_screenParams = screenParams;
	command.commandType = RenderCommandType::SetProjectionMatrix;
	command.param = setProjectionMatrixParam;
	m_cmdList.push_back(command);
}

void CommandBuffer::setViewMatrix(glm::mat4 viewMatrix)
{
	RenderCommand command = RenderCommand();
	SetViewMatrixParam* setViewMatrixParam = RenderCommandParamFactory::getInstance()->createParam<SetViewMatrixParam>();
	setViewMatrixParam->m_viewMatrix = viewMatrix;
	command.commandType = RenderCommandType::SetViewMatrix;
	command.param = setViewMatrixParam;
	m_cmdList.push_back(command);
}