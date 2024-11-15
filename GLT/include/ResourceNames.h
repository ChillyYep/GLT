#pragma once
class ResourceName
{
public:
	static const char* OpaqueRTName;
	static const char* PostProcessingRTName;
	static const char* PreProcessingRTName;
	static const char* ShadowMapRTName;
	static const char* DebugRTName;
	static const char* CaptureFBODebugRTName;
};

class ResourceCommonRegisterIndices {
public:
	static const unsigned int MainTexRegisterIndex;
	static const unsigned int ShadowMapRegisterIndex;

};