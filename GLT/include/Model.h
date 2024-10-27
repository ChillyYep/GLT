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
private:
	std::vector<Mesh*> m_meshes;
	std::vector<std::vector<Texture*>> m_textureResources;
};
