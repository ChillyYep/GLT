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

	bool isExecutable() override { return false; }

	void prepare() override;

	void execute() override;

	void destroy() override;

	void preDefine(std::string shaderName);
protected:
	virtual void preDefineProperties(Material* fullscreenMat) {}

	static SubMesh* createFullscreenTraingleMesh();
	static int s_postProcessingPassCount;
	static SubMesh* s_fullscreenTriangle;
	static RenderTexture* s_uberPostRT;
	static RenderTexture* s_prePostRT;

	RenderStateBlock m_renderStateBlock;
	Material* m_fullscreenMat;
	std::shared_ptr<Shader> m_fullscreenShader;

};