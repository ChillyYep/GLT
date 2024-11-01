#pragma once
#include <Cubemap.h>
#include <AssetUtils.h>
#include <PassBase.h>

class DrawSkyboxPass :public PassBase
{
public:
	bool isExecutable() override { return true; }
	void prepare() override
	{
		m_skybox = new Cubemap();
		m_skybox->m_name = "skybox";
		m_skybox->setInternalFormat(TextureInternalFormat::RGB8);
		m_skybox->setLevels(1);
		m_skybox->setPerChannelSize(TexturePerChannelSize::UNSIGNED_BYTE);
		m_skybox->setTextureFilter(TextureFilterMode::Linear_Mipmap_Linear);
		m_skybox->setFace(CubemapFace::Front, AssetUtils::getInstance()->loadTexture2D("Resources/skybox/front.jpg"));
		m_skybox->setFace(CubemapFace::Left, AssetUtils::getInstance()->loadTexture2D("Resources/skybox/left.jpg"));
		m_skybox->setFace(CubemapFace::Back, AssetUtils::getInstance()->loadTexture2D("Resources/skybox/back.jpg"));
		m_skybox->setFace(CubemapFace::Right, AssetUtils::getInstance()->loadTexture2D("Resources/skybox/right.jpg"));
		m_skybox->setFace(CubemapFace::Top, AssetUtils::getInstance()->loadTexture2D("Resources/skybox/top.jpg"));
		m_skybox->setFace(CubemapFace::Bottom, AssetUtils::getInstance()->loadTexture2D("Resources/skybox/bottom.jpg"));

		auto frontTex = m_skybox->getFace(CubemapFace::Front);
		if (frontTex != nullptr)
		{
			m_skybox->setWidth(frontTex->getWidth());
			m_skybox->setHeight(frontTex->getHeight());
		}

		LogicResourceManager::getInstance()->addResource(ResourceType::Texture, m_skybox);
	}

	void destroy() override
	{
		LogicResourceManager::getInstance()->destroyResource(ResourceType::Texture, m_skybox);
		if (m_skybox != nullptr)
		{
			delete m_skybox;
			m_skybox = nullptr;
		}
	}

	void execute() override
	{
	}
private:
	Cubemap* m_skybox;
};