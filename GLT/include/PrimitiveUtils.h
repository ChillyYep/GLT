#pragma once
#include <CommonDefine.h>
#include <Mesh.h>
class PrimitiveUtils
{
public:
	PrimitiveUtils() {};
	~PrimitiveUtils() {};
	static Mesh* createCube();

	static Mesh* createQuad();
private:

};
