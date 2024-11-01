#pragma once
#include <Texture.h>
#include <Model.h>
#include <Cubemap.h>
#include <3rdParty/stb_image/stb_image.h>
#include <unordered_map>
#include <Singleton.h>
#include <3rdParty/assimp/Importer.hpp>
#include <3rdParty/assimp/scene.h>
#include <3rdParty/assimp/postprocess.h>
#include <iostream>
#include <stack>

enum class AssetType {
	Texture,
	Model
};

class AssetUtils :public Singleton<AssetUtils>
{
public:
	AssetUtils()
	{
		m_assetDatabase[AssetType::Texture] = std::unordered_map<GLTUInt32, Object*>();
		m_assetDatabase[AssetType::Model] = std::unordered_map<GLTUInt32, Object*>();
	}
	~AssetUtils() {}

	inline bool isAssetLoaded(std::string filepath)
	{
		if (m_assetpath2instanceId.find(filepath) != m_assetpath2instanceId.end())
		{
			for (const auto& subDatabase : m_assetDatabase)
			{
				if (subDatabase.second.find(m_assetpath2instanceId[filepath]) != subDatabase.second.end())
				{
					return true;
				}
			}
		}
		return false;
	}

	Texture2D* loadTexture2D(std::string filepath);
	void unloadTexture2D(Texture2D* asset);

	Cubemap* loadCubemap(std::string filepath);
	void unloadCubemap(Cubemap* asset);

	Model* loadModel(std::string filepath);
	void unloadModel(Model* model);
private:
	void insertAssetToDatabase(std::string filepath, AssetType assetType, Object* asset);
	void removeAssetFromDatabase(Object* asset, AssetType assetType);

	void processNode(aiNode* node, const aiScene* scene, std::vector<aiMesh*>& meshes);
	SubMesh* processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture*> processTextures(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture*> processMaterialTextures(aiMaterial* mat, aiTextureType textureType, std::string typeName);

	std::unordered_map<GLTUInt32, std::string> m_instanceId2assetpath;
	std::unordered_map<std::string, GLTUInt32> m_assetpath2instanceId;
	std::unordered_map<AssetType, std::unordered_map<GLTUInt32, Object*>> m_assetDatabase;
};