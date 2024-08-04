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

	void SetRenderTarget(RenderTargetIdentifier* renderTargetIdentifier);
	void RequestRenderTexture(int width, int height, TextureFormat textureFormat, TextureFilterMode filterMode);
	void Release();
private:
	std::string m_name;
	std::vector<RenderCommand> m_cmdList;
};
