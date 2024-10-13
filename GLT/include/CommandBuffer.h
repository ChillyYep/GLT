#pragma once
#include <vector>
#include <string>
#include <RenderCommand.h>
#include <Texture.h>
#include <Mesh.h>
#include <Renderer.h>
#include <RenderResourceManagment.h>

class CommandBuffer
{
public:
	CommandBuffer()
	{
	}
	~CommandBuffer() {}

	void setRenderTarget(RenderTargetIdentifier* renderTargetIdentifier);

	void setGlobalTextureResource(TextureResourceIdentifier* textureResource);

	void clearColor(float r, float g, float b, float a);
	//void blitto(RenderTargetIdentifier* renderTargetIdentifier);
	void clear();

	void drawMesh(Mesh* mesh, Material* material, glm::mat4 modelMatrix);

	void drawRenderer(Renderer* renderer, Material* replacedMaterial = nullptr);

	void setViewport(int x, int y, int width, int height);

	void setProjectionMatrix(glm::mat4 projection);

	void setViewMatrix(glm::mat4 viewMatrix);

	const std::vector<RenderCommand>& getAllCmdList() const {
		return m_cmdList;
	}
private:
	std::string m_name;
	std::vector<RenderCommand> m_cmdList;
};
