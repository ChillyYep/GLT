#pragma once
#include <CommonDefine.h>
#include <Window.h>
#include <Renderer.h>
#include <Scene.h>
#include <Texture.h>
#include <Graphics.h>
#include <ShaderUtils.h>
#include <ScriptableRenderContext.h>

class RenderPipeline
{
public:
	RenderPipeline() {}
	~RenderPipeline() { }
	void Init();
	void UnInit();
	void Render();
private:
	void UpdateSceneProperties4Render();
	void PostUpdate();
	void UpdatePerFrameConstantBuffer();
	void UpdatePerCameraConstantBuffer(std::shared_ptr<Camera>& camera);
	void UpdateLightProperties(std::shared_ptr<Camera>& camera);
	void RenderPerObject(Renderer& renderObject, std::shared_ptr<Camera>& camera);

	template<typename T>
	void AppendNewObjects(ManagementCentreBase<T>* management, ResourceType resourceType)
	{
		auto&& newObjectList = management->GetNewObjects();
		const size_t newObjectCount = newObjectList.size();
		if (newObjectCount == 0)
		{
			return;
		}
		ResourceManager::getInstance()->RequestResource(management, newObjectList, resourceType);
		management->ClearNewList();
	}

	template<typename T>
	void ClearExpiredObjects(ManagementCentreBase<T>* management, ResourceType resourceType)
	{
		// É¾³ý
		auto&& expiredMeshInstanceIdList = management->GetExpiredObjectInstanceIds();
		size_t expiredMeshCount = expiredMeshInstanceIdList.size();
		if (expiredMeshCount == 0)
		{
			return;
		}
		ResourceManager::getInstance()->DestroyResource(management, expiredMeshInstanceIdList, resourceType);
		management->ClearExpiredList();
	}

	void ClearExpiredMeshBuffers(std::vector<std::shared_ptr<Scene>>& allScenes);
	void AppendNewMeshBuffers(std::vector<std::shared_ptr<Scene>>& allScenes);

	void ClearExpiredTextures(std::vector<std::shared_ptr<Scene>>& allScenes);
	void AppendNewTextures(std::vector<std::shared_ptr<Scene>>& allScenes);
	
	ScriptableRenderContext m_renderContext;

	std::vector<LightProperties> m_lightProperties;

	std::vector<std::shared_ptr<Scene>> m_allScenes;
};
