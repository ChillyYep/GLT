#pragma once
#include <ManagementCentreBase.h>
#include <memory>
#include <RenderTarget.h>
class RenderTargetManagementCentre :public ManagementCentreBase<std::shared_ptr<RenderTarget>>
{
public:
	RenderTargetManagementCentre() {}
	~RenderTargetManagementCentre() {}

private:

};