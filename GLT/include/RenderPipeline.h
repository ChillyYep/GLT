#pragma once
#include <CommonDefine.h>
#include <Window.h>
#include <Renderer.h>
#include <Scene.h>
#include <Texture.h>
#include <ShaderUtils.h>
#include <ScriptableRenderContext.h>
#include <SceneManager.h>

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

	template<typename T>
	void appendNewObjects(ManagementCentreBase<T>* management, ResourceType resourceType)
	{
		auto&& newObjectList = management->getNewObjects();
		const size_t newObjectCount = newObjectList.size();
		if (newObjectCount == 0)
		{
			return;
		}
		RenderResourceManager::getInstance()->requestResource(management, newObjectList, resourceType);
		management->clearNewList();
	}

	template<typename T>
	void clearExpiredObjects(ManagementCentreBase<T>* management, ResourceType resourceType)
	{
		// ɾ��
		auto&& expiredMeshInstanceIdList = management->getExpiredObjectInstanceIds();
		size_t expiredMeshCount = expiredMeshInstanceIdList.size();
		if (expiredMeshCount == 0)
		{
			return;
		}
		RenderResourceManager::getInstance()->destroyResource(management, expiredMeshInstanceIdList, resourceType);
		management->clearExpiredList();
	}

	ScriptableRenderContext m_renderContext;

	std::vector<LightProperties> m_lightProperties;

	std::vector<std::shared_ptr<Scene>> m_allScenes;

	CommandBuffer m_cmd;
};
