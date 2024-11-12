#pragma once
#include "PassBase.h"
#include <PrimitiveUtils.h>
#include <GLMath.h>
#include <Material.h>
#include <RenderResourceManagment.h>
#include <LogicResourceManager.h>
#include <ResourceNames.h>
#include <RenderTexture.h>

class PostProcessingPass :public PassBase
{
public:
	PostProcessingPass() {}
	~PostProcessingPass() {}

	void appointShader(std::string shaderName);
protected:
	virtual void updateMaterialProperties(Material* fullscreenMat) {}

	bool isExecutable() override { return false; }

	void onDefine() override;

	void onPrepare() override;

	bool isPrepared() override;

	void onExecute() override;

	void onDestroy() override;

	static SubMesh* createFullscreenTraingleMesh();
	static int s_postProcessingPassCount;
	static SubMesh* s_fullscreenTriangle;
	static RenderTexture* s_uberPostRT;
	static RenderTexture* s_prePostRT;

	Texture* m_colorRTTexture;
	RenderTargetIdentifier* m_colorRTIdentifier;
	RenderTargetIdentifier* m_uberRTIdentifier;

	RenderStateBlock m_renderStateBlock;
	Material* m_fullscreenMat;
	std::shared_ptr<Shader> m_fullscreenShader;

};