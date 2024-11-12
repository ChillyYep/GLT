#pragma once
#include <PostProcessingPass.h>

class ScreenSpaceAmbientOcclusionPass :public PostProcessingPass
{
public:
	ScreenSpaceAmbientOcclusionPass() {}
	~ScreenSpaceAmbientOcclusionPass() {}

	bool isExecutable() override { return true; }

	void onPrepare() override
	{
		PostProcessingPass::onPrepare();
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