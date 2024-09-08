#include "PrimitiveUtils.h"

Mesh* PrimitiveUtils::createCube()
{
	static const glm::vec4 cube_vertices[] = {
		// 前
		{-1.0f,-1.0f,1.0f,1.0f},
		{1.0f,-1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f,1.0f},
		{-1.0f,1.0f,1.0f,1.0f},
		// 后
		{-1.0f, -1.0f, -1.0f,1.0f},
		{-1.0f,  1.0f, -1.0f,1.0f},
		{1.0f,  1.0f, -1.0f,1.0f},
		{1.0f, -1.0f, -1.0f,1.0f},
		// 上
		{-1.0f,  1.0f, -1.0f,1.0f},
		{-1.0f,  1.0f,  1.0f,1.0f},
		{ 1.0f,  1.0f,  1.0f,1.0f},
		{ 1.0f,  1.0f, -1.0f,1.0f},
		// 下
		{-1.0f, -1.0f, -1.0f,1.0f},
		{1.0f, -1.0f,  -1.0f,1.0f},
		{1.0f, -1.0f,  1.0f,1.0f},
		{ -1.0f, -1.0f, 1.0f,1.0f},
		// 右
		{1.0f, -1.0f, -1.0f,1.0f},
		{ 1.0f,  1.0f, -1.0f,1.0f},
		{1.0f,  1.0f,  1.0f,1.0f},
		{1.0f, -1.0f,  1.0f,1.0f},
		// 左
		{-1.0f, -1.0f, -1.0f,1.0f},
		{-1.0f,  -1.0f, 1.0f,1.0f},
		{-1.0f,  1.0f,  1.0f,1.0f},
		{-1.0f, 1.0f,  -1.0f,1.0f},
	};


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
	static const GLTUInt16 cube_indices[] = {
		0,1,2,2,3,0,
		4,5,6,6,7,4,
		8,9,10,10,11,8,
		12,13,14,14,15,12,
		16,17,18,18,19,16,
		20,21,22,22,23,20,
	};

	static const glm::vec4 cube_normals[] = {
		// 前
		{0.0f,0.0f,1.0f,1.0f},
		{0.0f,0.0f,1.0f,1.0f},
		{0.0f,0.0f,1.0f,1.0f},
		{0.0f,0.0f,1.0f,1.0f},
		// 后
		{0.0f,0.0f,-1.0f,1.0f},
		{0.0f,0.0f,-1.0f,1.0f},
		{0.0f,0.0f,-1.0f,1.0f},
		{0.0f,0.0f,-1.0f,1.0f},
		// 上
		{0.0f,  1.0f,  0.0f,1.0f},
		{0.0f,  1.0f,  0.0f,1.0f},
		{0.0f,  1.0f,  0.0f,1.0f},
		{0.0f,  1.0f,  0.0f,1.0f},
		// 下
		{0.0f,  -1.0f,  0.0f,1.0f},
		{0.0f,  -1.0f,  0.0f,1.0f},
		{0.0f,  -1.0f,  0.0f,1.0f},
		{0.0f,  -1.0f,  0.0f,1.0f},
		// 右
		{1.0f,  0.0f, 0.0f,1.0f},
		{1.0f,  0.0f, 0.0f,1.0f},
		{1.0f,  0.0f, 0.0f,1.0f},
		{1.0f,  0.0f, 0.0f,1.0f},
		// 左
		{-1.0f,  0.0f, 0.0f,1.0f},
		{-1.0f,  0.0f, 0.0f,1.0f},
		{-1.0f,  0.0f, 0.0f,1.0f},
		{-1.0f,  0.0f, 0.0f,1.0f},
	};

	auto object = new Mesh(sizeof(cube_vertices) / sizeof(glm::vec4), sizeof(cube_indices) / sizeof(GLTUInt16));
	object->setVertices(cube_vertices);
	object->setIndices(cube_indices);
	object->setColors(cube_colors);
	object->setNormals(cube_normals);
	object->setUVs(cube_uvs);
	return object;
}

Mesh* PrimitiveUtils::createQuad()
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
		// 上
		{0.0f,  1.0f,  0.0f,1.0f},
		{0.0f,  1.0f,  0.0f,1.0f},
		{0.0f,  1.0f,  0.0f,1.0f},
		{0.0f,  1.0f,  0.0f,1.0f}
	};
	auto object = new Mesh(sizeof(plane_vertices) / sizeof(glm::vec4), sizeof(plane_indices) / sizeof(GLTUInt16));
	object->setVertices(plane_vertices);
	object->setIndices(plane_indices);
	object->setColors(plane_colors);
	object->setNormals(plane_normals);
	object->setUVs(plane_uvs);
	return object;
}