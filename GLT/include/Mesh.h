#pragma once
#include <CommonDefine.h>
#include <Object.h>

class Mesh :public Object
{
public:
	friend class MeshManagementCentre;
	/// <summary>
	/// 实时创建Mesh时可用的构造器
	/// </summary>
	/// <param name="verticesCount"></param>
	/// <param name="indicesCount"></param>
	Mesh(const GLsizei verticesCount, const GLsizei indicesCount) :Object()
	{
		_Allocate(verticesCount, indicesCount);
	}
	~Mesh() {
		_Deallocate();
	}
	inline void SetVertices(const glm::vec4 vertices[])
	{
		assert(m_readWrite);
		memcpy((void*)m_vertices, vertices, m_verticesCount * sizeof(glm::vec4));
	}
	inline void SetIndices(const GLushort indices[])
	{
		assert(m_readWrite);
		memcpy((void*)m_indices, indices, m_indicesCount * sizeof(GLushort));
	}
	inline void SetColors(const glm::vec4 colors[])
	{
		assert(m_readWrite);
		memcpy((void*)m_colors, colors, m_verticesCount * sizeof(glm::vec4));
	}

	inline void SetUVs(const glm::vec2 uvs[])
	{
		assert(m_readWrite);
		memcpy((void*)m_uvs, uvs, m_verticesCount * sizeof(glm::vec2));
	}

	inline void SetNormals(const glm::vec4 normals[])
	{
		assert(m_readWrite);
		memcpy((void*)m_normals, normals, m_verticesCount * sizeof(glm::vec4));
	}
	inline GLsizei GetVerticesCount() { return m_verticesCount; }
	inline GLsizei GetIndicesCount() const { return m_indicesCount; }

	inline const glm::vec4* GetVertices()
	{
		assert(m_readWrite);
		return m_vertices;
	}
	inline const glm::vec4* GetColors()
	{
		assert(m_readWrite);
		return m_colors;
	}
	inline const GLushort* GetIndices()
	{
		assert(m_readWrite);
		return m_indices;
	}
	inline const glm::vec2* GetUvs()
	{
		assert(m_readWrite);
		return m_uvs;
	}
	inline const glm::vec4* GetNormals()
	{
		assert(m_readWrite);
		return m_normals;
	}
	inline void MarkDirty() { m_dirty = true; }
	inline GLboolean IsDirty() { return m_dirty; }
	inline void ClearDirty() { m_dirty = false; }

	const static GLsizei VertexSize = sizeof(glm::vec4);
	const static GLsizei ColorSize = sizeof(glm::vec4);
	const static GLsizei IndexSize = sizeof(GLushort);
	const static GLsizei UVSize = sizeof(glm::vec2);
	const static GLsizei NormalSize = sizeof(glm::vec4);

	__GET_SET_PROPERTY__(CanReadWrite, GLboolean, m_readWrite)
private:
	inline void _Allocate(const GLsizei verticesCount, const GLsizei indicesCount) {
		if (m_allocated)
		{
			_Deallocate();
		}
		m_verticesCount = verticesCount;
		m_indicesCount = indicesCount;
		m_vertices = new glm::vec4[m_verticesCount];
		m_colors = new glm::vec4[m_verticesCount];
		m_indices = new GLushort[m_indicesCount];
		m_uvs = new glm::vec2[m_verticesCount];
		m_normals = new glm::vec4[m_verticesCount];

		m_dirty = false;
		m_readWrite = true;
	}
	inline void _Deallocate()
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
	GLboolean m_allocated;

	GLboolean m_dirty;

	GLboolean m_readWrite;

	GLsizei m_verticesCount;

	GLsizei m_indicesCount;

	const glm::vec4* m_vertices;
	const glm::vec2* m_uvs;
	const glm::vec4* m_normals;
	const glm::vec4* m_colors;
	const GLushort* m_indices;


};
