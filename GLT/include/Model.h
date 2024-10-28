#pragma once
#include <Object.h>
#include <vector>
#include <Mesh.h>

class Model :public Object
{
public:
	Model() {}
	~Model() {}
	void addMesh(Mesh* mesh, std::vector<Texture*> textures)
	{
		m_meshes.push_back(mesh);
		m_textureResources.push_back(textures);
	}
	Mesh* getMesh(int index)
	{
		if (index >= 0 && index < m_meshes.size())
		{
			return m_meshes[index];
		}
		return nullptr;
	}
	inline size_t getMeshCount() { return m_meshes.size(); }
private:
	std::vector<Mesh*> m_meshes;
	std::vector<std::vector<Texture*>> m_textureResources;
};
