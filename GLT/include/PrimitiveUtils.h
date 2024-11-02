#pragma once
#include <CommonDefine.h>
#include <Mesh.h>
enum class VertexProperty :unsigned int {
	Position = 1,
	Color = 2,
	Normal = 4,
	Texcoord1 = 8,
	Texcoord2 = 16,
	All = ((unsigned int)1 << 31) - (unsigned int)1
};
class PrimitiveUtils
{
public:
	PrimitiveUtils() {};
	~PrimitiveUtils() {};
	static SubMesh* createCube(VertexProperty needProperty = VertexProperty::All);

	static SubMesh* createQuad();
private:

};
