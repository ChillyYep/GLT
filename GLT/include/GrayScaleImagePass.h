#pragma once
#include <PostProcessingPass.h>

class GrayScaleImagePass :public PostProcessingPass
{
public:
	GrayScaleImagePass() {}
	~GrayScaleImagePass() {}

	bool isExecutable() override { return true; }

	void prepare() override
	{
		PostProcessingPass::prepare();
		preDefine("GrayScaleImage");
	}

	void execute() override
	{
		PostProcessingPass::execute();
	}

	void destroy() override
	{
		PostProcessingPass::destroy();
	}
protected:
	void preDefineProperties(Material* fullscreenMat) override
	{

	}
private:

};
