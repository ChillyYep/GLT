#pragma once
#include <CommonDefine.h>
#include <Window.h>
#include <Renderer.h>
#include <Scene.h>
#include <Texture.h>
#include <ShaderUtils.h>
#include <ScriptableRenderContext.h>
#include <SceneManager.h>
#include <PassBase.h>
#include <DrawOpaquePass.h>

class RenderPipeline
{
public:
	RenderPipeline() {}
	~RenderPipeline() { }
	void init();
	void uninit();
	void render();

	void drawMesh(Mesh* mesh, Material* material, glm::mat4 modelMatrix);
private:
	void updateSceneProperties4Render();
	void postUpdate();
	void updatePerFrameConstantBuffer();
	void updatePerCameraConstantBuffer(std::shared_ptr<Camera>& camera);
	void updateLightProperties(std::shared_ptr<Camera>& camera);
	void renderPerObject(Renderer& renderObject, std::shared_ptr<Camera>& camera);

	ScriptableRenderContext m_renderContext;

	std::vector<LightProperties> m_lightProperties;

	std::vector<std::shared_ptr<Scene>> m_allScenes;

	CommandBuffer m_cmd;

	PassList m_passList;
};
