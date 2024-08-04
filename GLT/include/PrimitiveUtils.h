#pragma once
#include <CommonDefine.h>
#include <Mesh.h>
#include <memory>
class PrimitiveUtils
{
public:
	PrimitiveUtils() {};
	~PrimitiveUtils() {};
	static std::shared_ptr<Mesh> CreateCube();

private:

};
