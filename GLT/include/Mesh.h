#pragma once
#include <CommonDefine.h>
#include <Object.h>
#include <GLMath.h>
#include <ResourceIdentifier.h>
struct Bound {
	glm::vec3 m_center;
	glm::vec3 m_extends;
};

class Mesh :public Object
{
public:
	friend class MeshManagementCentre;
	/// <summary>
	/// 实时创建Mesh时可用的构造器
	/// </summary>
	/// <param name="verticesCount"></param>
	/// <param name="indicesCount"></param>
	Mesh(const GLTSizei verticesCount, const GLTSizei indicesCount) :Object()
	{
		_allocate(verticesCount, indicesCount);
	}
	~Mesh() {
		_deallocate();
	}
	inline void setVertices(const glm::vec4 vertices[])
	{
		assert(m_readWrite);
		// 计算Bound
		m_bound = computeBound(vertices, m_verticesCount);
		memcpy((void*)m_vertices, vertices, m_verticesCount * sizeof(glm::vec4));
	}
	inline void setIndices(const GLTUInt16 indices[])
	{
		assert(m_readWrite);
		memcpy((void*)m_indices, indices, m_indicesCount * sizeof(GLTUInt16));
	}
	inline void setColors(const glm::vec4 colors[])
	{
		assert(m_readWrite);
		memcpy((void*)m_colors, colors, m_verticesCount * sizeof(glm::vec4));
	}

	inline void setUVs(const glm::vec2 uvs[])
	{
		assert(m_readWrite);
		memcpy((void*)m_uvs, uvs, m_verticesCount * sizeof(glm::vec2));
	}

	inline void setNormals(const glm::vec4 normals[])
	{
		assert(m_readWrite);
		memcpy((void*)m_normals, normals, m_verticesCount * sizeof(glm::vec4));
	}
	inline GLTSizei getVerticesCount() const { return m_verticesCount; }
	inline GLTSizei getIndicesCount() const { return m_indicesCount; }
	inline Bound getBound() const { return m_bound; }

	inline const glm::vec4* getVertices() const
	{
		assert(m_readWrite);
		return m_vertices;
	}
	inline const glm::vec4* getColors() const
	{
		assert(m_readWrite);
		return m_colors;
	}
	inline const GLTUInt16* getIndices() const
	{
		assert(m_readWrite);
		return m_indices;
	}
	inline const glm::vec2* getUvs() const
	{
		assert(m_readWrite);
		return m_uvs;
	}
	inline const glm::vec4* getNormals() const
	{
		assert(m_readWrite);
		return m_normals;
	}
	inline void markDirty() { m_dirty = true; }
	inline GLTBool isDirty() const { return m_dirty; }
	inline void clearDirty() { m_dirty = false; }

	static Bound computeBound(const glm::vec4 vertices[], GLTSizei verticesCount)
	{
		Bound bound;
		float minX = 99999999.0f, minY = 99999999.0f, minZ = 99999999.0f;
		float maxX = -99999999.0f, maxY = -99999999.0f, maxZ = -99999999.0f;
		for (int i = 0;i < verticesCount;++i)
		{
			if (vertices[i].x >= maxX)
			{
				maxX = vertices[i].x;
			}
			if (vertices[i].y >= maxY)
			{
				maxY = vertices[i].y;
			}
			if (vertices[i].z >= maxZ)
			{
				maxZ = vertices[i].z;
			}

			if (vertices[i].x <= minX)
			{
				minX = vertices[i].x;
			}
			if (vertices[i].y <= minY)
			{
				minY = vertices[i].y;
			}
			if (vertices[i].z <= minZ)
			{
				minZ = vertices[i].z;
			}
		}
		bound.m_center = glm::vec3(minX + maxX, minY + maxY, minZ + maxZ) / 2.0f;
		bound.m_extends = glm::vec3(maxX - minX, maxY - minY, maxZ - minZ) / 2.0f;
		return bound;
	}


	const static GLTSizei VertexSize = sizeof(glm::vec4);
	const static GLTSizei ColorSize = sizeof(glm::vec4);
	const static GLTSizei IndexSize = sizeof(GLTUInt16);
	const static GLTSizei UVSize = sizeof(glm::vec2);
	const static GLTSizei NormalSize = sizeof(glm::vec4);

	__GET_SET_PROPERTY__(CanReadWrite, GLTBool, m_readWrite)
private:

	void _allocate(const GLTSizei verticesCount, const GLTSizei indicesCount);
	void _deallocate();

	GLTBool m_allocated;

	GLTBool m_dirty;

	GLTBool m_readWrite;

	GLTSizei m_verticesCount;

	GLTSizei m_indicesCount;

	Bound m_bound;

	const glm::vec4* m_vertices;
	const glm::vec2* m_uvs;
	const glm::vec4* m_normals;
	const glm::vec4* m_colors;
	const GLTUInt16* m_indices;


};
