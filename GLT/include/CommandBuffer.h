#pragma once
#include <vector>
#include <string>
#include <RenderCommand.h>
#include <Texture.h>
#include <Mesh.h>
#include <ResourceManager.h>

class CommandBuffer
{
public:
	CommandBuffer()
	{
	}
	~CommandBuffer() {}

	void setRenderTarget(RenderTargetIdentifier* renderTargetIdentifier);
	void clearColor(float r, float g, float b, float a);
	//void blitto(RenderTargetIdentifier* renderTargetIdentifier);
	void clear();

	void drawMesh(Mesh* mesh, Material* material, glm::mat4 modelMatrix);

	void setViewport(int x, int y, int width, int height);

	const std::vector<RenderCommand>& getAllCmdList() const {
		return m_cmdList;
	}
private:
	std::string m_name;
	std::vector<RenderCommand> m_cmdList;
};
