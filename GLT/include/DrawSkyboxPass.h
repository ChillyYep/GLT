#pragma once
#include <Cubemap.h>
#include <AssetUtils.h>
#include <PassBase.h>
#include <PrimitiveUtils.h>

class DrawSkyboxPass :public PassBase
{
public:
	bool isExecutable() override { return true; }
	void prepare() override
	{
		PassBase::prepare();
		m_skybox = new Cubemap();
		m_skybox->m_name = "skybox";
		m_skybox->setInternalFormat(TextureInternalFormat::RGB8);
		m_skybox->setLevels(1);
		m_skybox->setPerChannelSize(TexturePerChannelSize::UNSIGNED_BYTE);
		m_skybox->setTextureFilter(TextureFilterMode::Linear_Mipmap_Linear);
		m_skybox->setWrapModeS(TextureWrapMode::ClampEdge);
		m_skybox->setWrapModeT(TextureWrapMode::ClampEdge);
		m_skybox->setWrapModeR(TextureWrapMode::ClampEdge);
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

		m_cubeMesh = PrimitiveUtils::createCube(VertexProperty::Position);
		//m_cubeMesh = PrimitiveUtils::createCube((VertexProperty)((unsigned int)VertexProperty::Position | (unsigned int)VertexProperty::Texcoord1));

		m_matPtr = new Material(std::shared_ptr<Shader>(new Shader("Skybox")));

		m_matPtr->setProperty(ShaderPropertyNames::SkyBox, std::shared_ptr<MaterialProperty>(new MaterialTextureProperty(m_skybox)));

		LogicResourceManager::getInstance()->addResource(ResourceType::Texture, m_skybox);
		LogicResourceManager::getInstance()->addResource(ResourceType::Mesh, m_cubeMesh);


		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Front;
		m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(true, true, true, true);
		m_renderStateBlock.m_depthState.m_depthRange = glm::vec2(0, 1);
		m_renderStateBlock.m_depthState.m_writable = false;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;
	}

	void destroy() override
	{
		LogicResourceManager::getInstance()->destroyResource(ResourceType::Texture, m_skybox);
		LogicResourceManager::getInstance()->destroyResource(ResourceType::Mesh, m_cubeMesh);
		if (m_skybox != nullptr)
		{
			delete m_skybox;
			m_skybox = nullptr;
		}
		if (m_cubeMesh != nullptr)
		{
			delete m_cubeMesh;
			m_cubeMesh = nullptr;
		}
		if (m_matPtr != nullptr)
		{
			delete m_matPtr;
			m_matPtr = nullptr;
		}
	}

	void execute() override
	{
		m_context->setRenderStateBlock(m_renderStateBlock);

		m_cmdBuffer.drawMesh(m_cubeMesh, m_matPtr, glm::identity<glm::mat4>() * glm::scale(glm::vec3(100.0f)));
		m_context->scheduleCommandBuffer(m_cmdBuffer);
		m_cmdBuffer.clear();
		m_context->submit();
	}
private:
	RenderStateBlock m_renderStateBlock;

	Material* m_matPtr;

	Cubemap* m_skybox;

	SubMesh* m_cubeMesh;
};