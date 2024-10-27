#include "AssetUtils.h"

Texture2D* AssetUtils::loadTexture2D(std::string filepath)
{
	if (isAssetLoaded(filepath))
	{
		return static_cast<Texture2D*>(m_assetDatabase[AssetType::Texture][m_assetpath2instanceId[filepath]]);
	}
	int width, height, channels;
	GLTUByte* imageData = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
	if (imageData != nullptr)
	{
		Texture2D* tex2D = new Texture2D();
		tex2D->setWidth(width);
		tex2D->setHeight(height);
		if (channels == 1)
		{
			tex2D->setExternalFormat(TextureExternalFormat::RED);
		}
		else if (channels == 2)
		{
			tex2D->setExternalFormat(TextureExternalFormat::RG);
		}
		else if (channels == 3)
		{
			tex2D->setExternalFormat(TextureExternalFormat::RGB);
		}
		else if (channels == 4)
		{
			tex2D->setExternalFormat(TextureExternalFormat::RGBA);
		}
		tex2D->setData(imageData, width * height * channels);
		stbi_image_free(imageData);
		imageData = nullptr;
		insertAssetToDatabase(filepath, AssetType::Texture, tex2D);
		return tex2D;
	}
	return nullptr;
}

void AssetUtils::unloadTexture2D(Texture2D* asset)
{
	if (asset == nullptr || m_instanceId2assetpath.find(asset->getInstanceId()) == m_instanceId2assetpath.end())
	{
		return;
	}
	removeAssetFromDatabase(asset, AssetType::Texture);
}

void AssetUtils::insertAssetToDatabase(std::string filepath, AssetType assetType, Object* asset)
{
	auto instanceId = asset->getInstanceId();
	m_assetpath2instanceId[filepath] = instanceId;
	m_instanceId2assetpath[instanceId] = filepath;
	m_assetDatabase[assetType][instanceId] = asset;
}

void AssetUtils::removeAssetFromDatabase(Object* asset, AssetType assetType)
{
	auto instanceId = asset->getInstanceId();
	auto assetPath = m_instanceId2assetpath[instanceId];
	if (m_assetDatabase.find(assetType) != m_assetDatabase.end())
	{
		auto rawAsset = m_assetDatabase[assetType][instanceId];
		assert(rawAsset == asset);
		m_instanceId2assetpath.erase(instanceId);
		m_assetpath2instanceId.erase(assetPath);
		m_assetDatabase[assetType].erase(instanceId);
		delete asset;
	}
}

void AssetUtils::processNode(aiNode* node, const aiScene* scene, std::vector<aiMesh*> meshes)
{
	// 获取节点上Mesh
	for (int i = 0;i < node->mNumMeshes;++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(mesh);
	}
	// 处理子节点
	for (int i = 0;i < node->mNumChildren;++i)
	{
		processNode(node->mChildren[i], scene, meshes);
	}
}
Mesh* AssetUtils::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<glm::vec4> vertices(mesh->mNumVertices);
	std::vector<unsigned short> indices(mesh->mNumFaces * 3);
	Mesh* customMesh = new Mesh(mesh->mNumVertices, mesh->mNumFaces * 3);
	for (int i = 0;i < mesh->mNumVertices;++i)
	{
		auto aiVertex = mesh->mVertices[i];
		vertices[i] = glm::vec4(aiVertex.x, aiVertex.y, aiVertex.z, 1.f);
	}
	customMesh->setVertices(vertices.data());
	for (int i = 0;i < mesh->mNumFaces;++i)
	{
		auto aiFace = mesh->mFaces[i];
		for (int j = 0;j < aiFace.mNumIndices;++j)
		{
			indices[j] = aiFace.mIndices[j];
		}
	}
	customMesh->setIndices(indices.data());
	if (mesh->HasNormals())
	{
		std::vector<glm::vec4> normals(mesh->mNumVertices);
		for (int i = 0;i < mesh->mNumVertices;++i)
		{
			auto aiNormal = mesh->mNormals[i];
			normals[i] = glm::vec4(aiNormal.x, aiNormal.y, aiNormal.z, 1.f);
		}
		customMesh->setNormals(normals.data());
	}

	if (mesh->mColors[0] != nullptr)
	{
		std::vector<glm::vec4> colors(mesh->mNumVertices);
		auto aiColors = mesh->mColors[0];
		for (int i = 0;i < mesh->mNumVertices;++i)
		{
			auto aiColor = aiColors[i];
			colors[i] = glm::vec4(aiColor.r, aiColor.g, aiColor.b, aiColor.a);
		}
		customMesh->setColors(colors.data());
	}
	if (mesh->mTextureCoords[0] != nullptr)
	{
		std::vector<glm::vec2> uvs(mesh->mNumVertices);
		auto aiUVs = mesh->mTextureCoords[0];
		for (int i = 0;i < mesh->mNumVertices;++i)
		{
			auto aiUv = aiUVs[i];
			uvs[i] = glm::vec2(aiUv.x, aiUv.y);
		}
		customMesh->setUVs(uvs.data());
	}
	return customMesh;
}
std::vector<Texture*> AssetUtils::processTextures(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Texture*> textures;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		auto diffuseTextures = processMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
		auto specularTextures = processMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
		textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
	}
	return textures;
}
std::vector<Texture*> AssetUtils::processMaterialTextures(aiMaterial* mat, aiTextureType textureType, std::string typeName)
{
	auto subDatabase = m_assetDatabase[AssetType::Texture];
	std::vector<Texture*> textures;

	for (int i = 0;i < mat->GetTextureCount(textureType);++i)
	{
		aiString path;
		mat->GetTexture(textureType, i, &path);
		std::string localPath = std::string(path.C_Str());
		bool textureLoaded = isAssetLoaded(localPath);
		auto tex2D = loadTexture2D(localPath);
		if (textureLoaded)
		{
			assert(tex2D->m_name == typeName);
		}
		tex2D->m_name = typeName;
		textures.push_back(tex2D);
	}
	return textures;
}

Model* AssetUtils::loadModel(std::string filepath)
{
	if (isAssetLoaded(filepath))
	{
		return static_cast<Model*>(m_assetDatabase[AssetType::Model][m_assetpath2instanceId[filepath]]);
	}
	Model* model = new Model();
	// Assimp原理 https://learnopengl-cn.readthedocs.io/zh/latest/03%20Model%20Loading/01%20Assimp/
	Assimp::Importer importer;
	// 1、加载scene
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (scene == nullptr || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return nullptr;
	}
	// 2、处理node
	std::vector<aiMesh*> allMeshes;
	processNode(scene->mRootNode, scene, allMeshes);

	// 3、处理Mesh
	for (int i = 0;i < allMeshes.size();++i)
	{
		auto mesh = allMeshes[i];
		Mesh* customMesh = processMesh(mesh, scene);
		std::vector<Texture*> textures = processTextures(mesh, scene);
		model->addMesh(customMesh, textures);
	}
	insertAssetToDatabase(filepath, AssetType::Model, model);
	return model;
}
void AssetUtils::unloadModel(Model* model)
{
	if (model == nullptr || m_instanceId2assetpath.find(model->getInstanceId()) == m_instanceId2assetpath.end())
	{
		return;
	}
	removeAssetFromDatabase(model, AssetType::Model);
}