#pragma once
#include <CommonDefine.h>
#include <Mesh.h>
class PrimitiveUtils
{
public:
	PrimitiveUtils() {};
	~PrimitiveUtils() {};
	static SubMesh* createCube();

	static SubMesh* createQuad();
private:

};
