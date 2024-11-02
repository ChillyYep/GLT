#include "PrimitiveUtils.h"

SubMesh* PrimitiveUtils::createCube(VertexProperty needProperty)
{
	static const glm::vec4 cube_vertices[] = {
		// ǰ
		{-1.0f,-1.0f,1.0f,1.0f},
		{1.0f,-1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f,1.0f},
		{-1.0f,1.0f,1.0f,1.0f},
		// ��
		{-1.0f, -1.0f, -1.0f,1.0f},
		{-1.0f,  1.0f, -1.0f,1.0f},
		{1.0f,  1.0f, -1.0f,1.0f},
		{1.0f, -1.0f, -1.0f,1.0f},
		// ��
		{-1.0f,  1.0f, -1.0f,1.0f},
		{-1.0f,  1.0f,  1.0f,1.0f},
		{ 1.0f,  1.0f,  1.0f,1.0f},
		{ 1.0f,  1.0f, -1.0f,1.0f},
		// ��
		{-1.0f, -1.0f, -1.0f,1.0f},
		{1.0f, -1.0f,  -1.0f,1.0f},
		{1.0f, -1.0f,  1.0f,1.0f},
		{ -1.0f, -1.0f, 1.0f,1.0f},
		// ��
		{1.0f, -1.0f, -1.0f,1.0f},
		{ 1.0f,  1.0f, -1.0f,1.0f},
		{1.0f,  1.0f,  1.0f,1.0f},
		{1.0f, -1.0f,  1.0f,1.0f},
		// ��
		{-1.0f, -1.0f, -1.0f,1.0f},
		{-1.0f,  -1.0f, 1.0f,1.0f},
		{-1.0f,  1.0f,  1.0f,1.0f},
		{-1.0f, 1.0f,  -1.0f,1.0f},
	};
	static const GLTUInt16 cube_indices[] = {
		0,1,2,2,3,0,
		4,5,6,6,7,4,
		8,9,10,10,11,8,
		12,13,14,14,15,12,
		16,17,18,18,19,16,
		20,21,22,22,23,20,
	};
	auto object = new SubMesh(sizeof(cube_vertices) / sizeof(glm::vec4), sizeof(cube_indices) / sizeof(GLTUInt16));

	object->setVertices(cube_vertices);
	object->setIndices(cube_indices);
	if (((unsigned int)needProperty & (unsigned int)VertexProperty::Color) > 0)
	{
		static const glm::vec4 cube_colors[] = {
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},

			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},

			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},

			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},

			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},

			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
			{1.0f,1.0f,1.0f,1.0f},
		};
		object->setColors(cube_colors);
	}

	if (((unsigned int)needProperty & (unsigned int)VertexProperty::Texcoord1) > 0)
	{
		static const glm::vec2 cube_uvs[] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f},

		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f},

		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f},

		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f},

		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f},

		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f},
		{0.0f,1.0f},
		};
		object->setUVs(cube_uvs);
	}


	if (((unsigned int)needProperty & (unsigned int)VertexProperty::Normal) > 0)
	{
		static const glm::vec4 cube_normals[] = {
			// ǰ
			{0.0f,0.0f,1.0f,1.0f},
			{0.0f,0.0f,1.0f,1.0f},
			{0.0f,0.0f,1.0f,1.0f},
			{0.0f,0.0f,1.0f,1.0f},
			// ��
			{0.0f,0.0f,-1.0f,1.0f},
			{0.0f,0.0f,-1.0f,1.0f},
			{0.0f,0.0f,-1.0f,1.0f},
			{0.0f,0.0f,-1.0f,1.0f},
			// ��
			{0.0f,  1.0f,  0.0f,1.0f},
			{0.0f,  1.0f,  0.0f,1.0f},
			{0.0f,  1.0f,  0.0f,1.0f},
			{0.0f,  1.0f,  0.0f,1.0f},
			// ��
			{0.0f,  -1.0f,  0.0f,1.0f},
			{0.0f,  -1.0f,  0.0f,1.0f},
			{0.0f,  -1.0f,  0.0f,1.0f},
			{0.0f,  -1.0f,  0.0f,1.0f},
			// ��
			{1.0f,  0.0f, 0.0f,1.0f},
			{1.0f,  0.0f, 0.0f,1.0f},
			{1.0f,  0.0f, 0.0f,1.0f},
			{1.0f,  0.0f, 0.0f,1.0f},
			// ��
			{-1.0f,  0.0f, 0.0f,1.0f},
			{-1.0f,  0.0f, 0.0f,1.0f},
			{-1.0f,  0.0f, 0.0f,1.0f},
			{-1.0f,  0.0f, 0.0f,1.0f},
		};
		object->setNormals(cube_normals);
	}

	
	return object;
}

SubMesh* PrimitiveUtils::createQuad()
{
	static const glm::vec4 plane_vertices[] = {
		{-1.0f,  0.0f, -1.0f,1.0f},
		{-1.0f,  0.0f,  1.0f,1.0f},
		{ 1.0f,  0.0f,  1.0f,1.0f},
		{ 1.0f,  0.0f, -1.0f,1.0f}
	};


	static const glm::vec4 plane_colors[] = {
		{1.0f,1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f,1.0f}
	};

	static const glm::vec2 plane_uvs[] = {
		{0.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f},
		{1.0f,0.0f}
	};
	static const GLTUInt16 plane_indices[] = {
		0,1,2,2,3,0
	};

	static const glm::vec4 plane_normals[] = {
		// ��
		{0.0f,  1.0f,  0.0f,1.0f},
		{0.0f,  1.0f,  0.0f,1.0f},
		{0.0f,  1.0f,  0.0f,1.0f},
		{0.0f,  1.0f,  0.0f,1.0f}
	};
	auto object = new SubMesh(sizeof(plane_vertices) / sizeof(glm::vec4), sizeof(plane_indices) / sizeof(GLTUInt16));
	object->setVertices(plane_vertices);
	object->setIndices(plane_indices);
	object->setColors(plane_colors);
	object->setNormals(plane_normals);
	object->setUVs(plane_uvs);
	return object;
}