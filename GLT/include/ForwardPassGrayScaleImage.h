#pragma once
#include <ForwardPassPostProcessing.h>

class ForwardPassGrayScaleImage :public ForwardPassPostProcessing
{
public:
	ForwardPassGrayScaleImage() {}
	~ForwardPassGrayScaleImage() {}

	bool isExecutable() override { return true; }


protected:
	void updateMaterialProperties(Material* fullscreenMat) override {}
	void onPrepare() override
	{
		ForwardPassPostProcessing::onPrepare();
		appointShader("GrayScaleImage");
	}

	void onExecute() override
	{
		ForwardPassPostProcessing::onExecute();
	}

	void onDestroy() override
	{
		ForwardPassPostProcessing::onDestroy();
	}
private:

};
