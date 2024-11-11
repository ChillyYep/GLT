#pragma once
#include <RenderPathBase.h>
#include <DrawOpaquePass.h>
#include <DrawTransparentPass.h>
#include <SimpleShadowMapPass.h>
#include <PostProcessingPass.h>
#include <CaptureFBOPass.h>
#include <DrawSkyboxPass.h>
#include <ScreenSpaceAmbientOcclusionPass.h>
#include <GrayScaleImagePass.h>

class ForwardRenderPath :public RenderPathBase
{
public:
	ForwardRenderPath() {}
	~ForwardRenderPath() {}

	void init(ScriptableRenderContext* renderContext, RenderData* renderData, CommandBuffer* commandBuffer) override;
private:

};
