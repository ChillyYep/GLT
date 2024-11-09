#pragma once
#include <CommonDefine.h>
#include <Window.h>
#include <Scene.h>
#include <Texture.h>
#include <ShaderUtils.h>
#include <ScriptableRenderContext.h>
#include <SceneManager.h>
#include <PassBase.h>
#include <ForwardRenderPath.h>

class RenderPipeline
{
public:
	RenderPipeline() {}
	~RenderPipeline() { }
	void init();
	void uninit();
	void render();

	void drawMesh(SubMesh* mesh, Material* material, glm::mat4 modelMatrix);
private:
	void postUpdate();
	void updatePerFrameConstantBuffer();
	void updatePerCameraConstantBuffer(CameraData& cameraData);
	void updateLightProperties();

	RenderPathBase* m_renderPath;

	ScriptableRenderContext m_renderContext;

	std::vector<LightProperties> m_lightProperties;

	std::vector<std::shared_ptr<Scene>> m_allScenes;

	CommandBuffer m_cmd;

	RenderData m_renderData;
};
