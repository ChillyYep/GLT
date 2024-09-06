#pragma once
#include <ManagementCentreBase.h>
#include <memory>
#include <RenderTarget.h>
class RenderTargetManagementCentre :public ManagementCentreBase<RenderTarget*>
{
public:
	RenderTargetManagementCentre() {}
	~RenderTargetManagementCentre() {}

private:

};