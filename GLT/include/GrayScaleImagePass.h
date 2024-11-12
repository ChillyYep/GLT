#pragma once
#include <PostProcessingPass.h>

class GrayScaleImagePass :public PostProcessingPass
{
public:
	GrayScaleImagePass() {}
	~GrayScaleImagePass() {}

	bool isExecutable() override { return true; }


protected:
	void updateMaterialProperties(Material* fullscreenMat) override {}
	void onPrepare() override
	{
		PostProcessingPass::onPrepare();
		appointShader("GrayScaleImage");
	}

	void onExecute() override
	{
		PostProcessingPass::onExecute();
	}

	void onDestroy() override
	{
		PostProcessingPass::onDestroy();
	}
private:

};
