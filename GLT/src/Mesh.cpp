#include "Mesh.h"
void Mesh::_allocate(const GLTSizei verticesCount, const GLTSizei indicesCount) {
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

void  Mesh::_deallocate()
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