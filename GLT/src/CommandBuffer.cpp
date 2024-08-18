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
	RenderCommand command = RenderCommand();
	DrawMeshParam* drawMeshParam = RenderCommandParamFactory::getInstance()->createParam<DrawMeshParam>();
	drawMeshParam->m_meshPtr = mesh;
	drawMeshParam->m_materialPtr = material;
	drawMeshParam->m_modelMatrix = modelMatrix;
	drawMeshParam->m_meshResourceIdentifier = ResourceManager::getInstance()->getMeshResource(mesh->getInstanceId());
	drawMeshParam->m_textureResources = ResourceManager::getInstance()->getTextureResources();
	command.commandType = RenderCommandType::DrawMesh;
	command.param = drawMeshParam;
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