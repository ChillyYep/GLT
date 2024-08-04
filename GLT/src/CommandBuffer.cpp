#include "CommandBuffer.h"

void CommandBuffer::RequestRenderTexture(int width, int height, TextureFormat textureFormat, TextureFilterMode filterMode)
{
	RenderCommand command = RenderCommand();
	RequestRenderTextureParam* renderTextureParam = RenderCommandParamFactory::getInstance()->createParam<RequestRenderTextureParam>();
	renderTextureParam->width = width;
	renderTextureParam->height = height;
	renderTextureParam->textureFormat = textureFormat;
	renderTextureParam->filterMode = filterMode;
	command.commandType = RenderCommandType::RequestRenderTexture;
	command.param = renderTextureParam;
	m_cmdList.push_back(command);
}

void CommandBuffer::SetRenderTarget(RenderTargetIdentifier* renderTargetIdentifier)
{
	assert(renderTargetIdentifier != nullptr);
	RenderCommand command = RenderCommand();
	SetRenderTargetParam* setRenderTargetParam = RenderCommandParamFactory::getInstance()->createParam<SetRenderTargetParam>();
	setRenderTargetParam->identifier = renderTargetIdentifier;

	command.commandType = RenderCommandType::SetRenderTarget;
	command.param = setRenderTargetParam;
	m_cmdList.push_back(command);
}

void CommandBuffer::Release()
{
	for (int i = 0;i < m_cmdList.size();++i)
	{
		RenderCommandParamFactory::getInstance()->releaseParam(m_cmdList[i].param);
	}
}