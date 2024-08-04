#pragma once
#include <CommonDefine.h>
#include <Object.h>
#include <GLMath.h>

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
		_Allocate(verticesCount, indicesCount);
	}
	~Mesh() {
		_deallocate();
	}
	inline void setVertices(const glm::vec4 vertices[])
	{
		assert(m_readWrite);
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
	inline GLTSizei getVerticesCount() { return m_verticesCount; }
	inline GLTSizei getIndicesCount() const { return m_indicesCount; }

	inline const glm::vec4* getVertices()
	{
		assert(m_readWrite);
		return m_vertices;
	}
	inline const glm::vec4* getColors()
	{
		assert(m_readWrite);
		return m_colors;
	}
	inline const GLTUInt16* getIndices()
	{
		assert(m_readWrite);
		return m_indices;
	}
	inline const glm::vec2* getUvs()
	{
		assert(m_readWrite);
		return m_uvs;
	}
	inline const glm::vec4* getNormals()
	{
		assert(m_readWrite);
		return m_normals;
	}
	inline void markDirty() { m_dirty = true; }
	inline GLTBool isDirty() { return m_dirty; }
	inline void clearDirty() { m_dirty = false; }

	const static GLTSizei VertexSize = sizeof(glm::vec4);
	const static GLTSizei ColorSize = sizeof(glm::vec4);
	const static GLTSizei IndexSize = sizeof(GLTUInt16);
	const static GLTSizei UVSize = sizeof(glm::vec2);
	const static GLTSizei NormalSize = sizeof(glm::vec4);

	__GET_SET_PROPERTY__(CanReadWrite, GLTBool, m_readWrite)
private:
	inline void _Allocate(const GLTSizei verticesCount, const GLTSizei indicesCount) {
		if (m_allocated)
		{
			_deallocate();
		}
		m_verticesCount = verticesCount;
		m_indicesCount = indicesCount;
		m_vertices = new glm::vec4[m_verticesCount];
		m_colors = new glm::vec4[m_verticesCount];
		m_indices = new GLTUInt16[m_indicesCount];
		m_uvs = new glm::vec2[m_verticesCount];
		m_normals = new glm::vec4[m_verticesCount];

		m_dirty = false;
		m_readWrite = true;
	}
	inline void _deallocate()
	{
		if (m_allocated)
		{
			if (m_vertices != nullptr)
			{
				delete[] m_vertices;
				m_vertices = nullptr;
			}
			if (m_colors != nullptr)
			{
				delete[] m_colors;
				m_colors = nullptr;
			}
			if (m_indices != nullptr)
			{
				delete[] m_indices;
				m_indices = nullptr;
			}
			if (m_uvs != nullptr)
			{
				delete[] m_uvs;
				m_uvs = nullptr;
			}
			if (m_normals != nullptr)
			{
				delete[] m_normals;
				m_normals = nullptr;
			}
			m_allocated = false;
		}
	}
	GLTBool m_allocated;

	GLTBool m_dirty;

	GLTBool m_readWrite;

	GLTSizei m_verticesCount;

	GLTSizei m_indicesCount;

	const glm::vec4* m_vertices;
	const glm::vec2* m_uvs;
	const glm::vec4* m_normals;
	const glm::vec4* m_colors;
	const GLTUInt16* m_indices;


};
