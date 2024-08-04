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
	void requestRenderTexture(int width, int height, TextureFormat textureFormat, TextureFilterMode filterMode);
	void release();
private:
	std::string m_name;
	std::vector<RenderCommand> m_cmdList;
};
