#pragma once
#include <PassBase.h>
class RenderPathBase
{
public:
	RenderPathBase() {}
	~RenderPathBase() {}

	virtual void renderGlobally();
	virtual void renderPerCamera(CameraData* cameraData);

	virtual void init(ScriptableRenderContext* renderContext, RenderData* renderData, CommandBuffer* commandBuffer);

	virtual void uninit();

	inline bool hasGlobalPass() { return m_globalPassList.size() > 0; }

	inline bool hasPerCameraPass() { return m_perCameraPassList.size() > 0; }

protected:
	CommandBuffer* m_cmd;

	RenderData* m_renderData;

	ScriptableRenderContext* m_renderContext;

	PassList m_globalPassList;

	PassList m_perCameraPassList;
};