#pragma once
#include <RenderBuffer.h>
#include <ManagementCentreBase.h>
#include <memory>

class RenderBufferManagementCentre :public ManagementCentreBase<RenderBuffer*>
{
public:
	RenderBufferManagementCentre() {}
	~RenderBufferManagementCentre() {}

private:

};
