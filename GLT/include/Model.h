#pragma once
#include <Object.h>
#include <vector>
#include <Mesh.h>
#include <Texture.h>

class Model :public Object
{
public:
	Model() {}
	~Model() {}
	void addMesh(SubMesh* mesh, std::vector<Texture*> textures)
	{
		m_meshes.push_back(mesh);
		m_textureResources.push_back(textures);
	}
	SubMesh* getMesh(int index)
	{
		if (index < m_meshes.size() && index >= 0)
		{
			return m_meshes[index];
		}
		return nullptr;
	}
	inline std::vector<SubMesh*> getSubMeshes() { return m_meshes; }
	inline const size_t getMeshCount() { return m_meshes.size(); }
private:
	std::vector<SubMesh*> m_meshes;
	std::vector<std::vector<Texture*>> m_textureResources;
};
