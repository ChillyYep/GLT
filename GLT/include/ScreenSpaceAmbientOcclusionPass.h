#pragma once
#include <PostProcessingPass.h>

class ScreenSpaceAmbientOcclusionPass :public PostProcessingPass
{
public:
	ScreenSpaceAmbientOcclusionPass() {}
	~ScreenSpaceAmbientOcclusionPass() {}

	bool isExecutable() override { return true; }

	void prepare() override
	{
		PostProcessingPass::prepare();
	}

	void execute() override
	{
		PostProcessingPass::execute();
	}

	void destroy() override
	{
		PostProcessingPass::destroy();
	}
private:

};