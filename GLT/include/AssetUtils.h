#pragma once
#include <Texture.h>
#include <Mesh.h>
#include <3rdParty/stb_image/stb_image.h>
#include <unordered_map>
#include <Singleton.h>

class AssetUtils :public Singleton<AssetUtils>
{
public:
	AssetUtils() {}
	~AssetUtils() {}

	inline bool isAssetLoaded(std::string filepath)
	{
		if (m_assetpath2instanceId.find(filepath) != m_assetpath2instanceId.end() &&
			m_assetDatabase.find(m_assetpath2instanceId[filepath]) != m_assetDatabase.end())
		{
			return true;
		}
		return false;
	}

	Texture2D* loadTexture2D(std::string filepath)
	{
		if (isAssetLoaded(filepath))
		{
			return static_cast<Texture2D*>(m_assetDatabase[m_assetpath2instanceId[filepath]]);
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
			insertAsset(filepath, tex2D);
			return tex2D;
		}
		return nullptr;
	}

	void unloadTexture2D(Texture2D* asset)
	{
		if (asset == nullptr || m_instanceId2assetpath.find(asset->getInstanceId()) == m_instanceId2assetpath.end())
		{
			return;
		}
		asset->setData(nullptr);
		removeAsset(asset);
	}
private:
	inline void insertAsset(std::string filepath, Object* asset)
	{
		auto instanceId = asset->getInstanceId();
		m_assetpath2instanceId[filepath] = instanceId;
		m_instanceId2assetpath[instanceId] = filepath;
		m_assetDatabase[instanceId] = asset;
	}

	inline void removeAsset(Object* asset)
	{
		auto instanceId = asset->getInstanceId();
		auto assetPath = m_instanceId2assetpath[instanceId];
		auto rawAsset = m_assetDatabase[instanceId];
		assert(rawAsset == asset);
		m_instanceId2assetpath.erase(instanceId);
		m_assetpath2instanceId.erase(assetPath);
		m_assetDatabase.erase(instanceId);
		delete asset;
	}
	std::unordered_map<GLTUInt32, std::string> m_instanceId2assetpath;
	std::unordered_map<std::string, GLTUInt32> m_assetpath2instanceId;
	std::unordered_map<GLTUInt32, Object*> m_assetDatabase;
};