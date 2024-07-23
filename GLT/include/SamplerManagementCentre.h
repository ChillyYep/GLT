#pragma once
#include <ManagementCentreBase.h>
#include <Sampler.h>

class SamplerManagementCentre :public ManagementCentreBase<std::shared_ptr<Sampler>>
{
public:
	SamplerManagementCentre() {}
	~SamplerManagementCentre() {}

private:

};