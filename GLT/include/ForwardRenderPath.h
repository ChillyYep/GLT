#pragma once
#include <RenderPathBase.h>
#include <ForwardPassDrawOpaque.h>
#include <ForwardPassDrawTransparent.h>
#include <ForwardPassSimpleShadowMap.h>
#include <ForwardPassPostProcessing.h>
#include <PassCaptureFBO.h>
#include <ForwardPassDrawSkybox.h>
#include <ForwardPassScreenSpaceAmbientOcclusion.h>
#include <ForwardPassGrayScaleImage.h>

class ForwardRenderPath :public RenderPathBase
{
public:
	ForwardRenderPath() {}
	~ForwardRenderPath() {}

	void init(ScriptableRenderContext* renderContext, RenderData* renderData, CommandBuffer* commandBuffer) override;
private:

};
