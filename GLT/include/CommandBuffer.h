#pragma once
#include <vector>
#include <string>
#include <RenderCommand.h>
#include <Texture.h>

class CommandBuffer
{
public:
	CommandBuffer()
	{
	}
	~CommandBuffer() {}

	void setRenderTarget(RenderTargetIdentifier* renderTargetIdentifier);
	void clearColor(float r,float g,float b,float a);
	//void blitto(RenderTargetIdentifier* renderTargetIdentifier);
	void requestRenderTexture(int width, int height, TextureFormat textureFormat, TextureFilterMode filterMode);
	void release();

	void clear();

	const std::vector<RenderCommand>& getAllCmdList() const {
		return m_cmdList;
	}
private:
	std::string m_name;
	std::vector<RenderCommand> m_cmdList;
};
