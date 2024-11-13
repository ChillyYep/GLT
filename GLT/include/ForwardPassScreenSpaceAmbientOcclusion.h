#pragma once
#include <ForwardPassPostProcessing.h>
#include <random>
#include <RenderResourceManagment.h>

class ForwardPassScreenSpaceAmbientOcclusion :public ForwardPassPostProcessing
{
public:
	ForwardPassScreenSpaceAmbientOcclusion() {}
	~ForwardPassScreenSpaceAmbientOcclusion() {}

	bool isExecutable() override { return true; }

	void onDefine() override
	{
		ForwardPassPostProcessing::onDefine();

		// 随机浮点数，范围0.0~1.0
		std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
		std::default_random_engine generator;
		const int randomSampleCount = 64;
		m_ssaoKernel.resize(randomSampleCount);
		for (unsigned int i = 0; i < randomSampleCount; ++i)
		{
			// 随机采样点，分布在z正方向半球内
			glm::vec4 sample(randomFloats(generator) * 2.0f - 1.0f,
				randomFloats(generator) * 2.0f - 1.0f,
				randomFloats(generator) * 2.0f - 1.0f, 0.0f);
			sample = glm::normalize(sample);
			float scale = float(i) / 64.0f;
			// 使采样点分布更靠近片段表面
			scale = glm::lerp(0.1f, 1.0f, scale * scale);
			sample *= scale * randomFloats(generator);
			m_ssaoKernel.push_back(sample);
		}
	}
	void updateMaterialProperties(Material* fullscreenMat) override
	{
		fullscreenMat->setProperty("samples", std::shared_ptr<MaterialVector4ArrayProperty>(new MaterialVector4ArrayProperty(m_ssaoKernel)));
	}
	void onPrepare() override
	{
		ForwardPassPostProcessing::onPrepare();
		appointShader("SSAOForward");
	}

	bool isPrepared() override
	{
		if (!ForwardPassPostProcessing::isPrepared())
		{
			return false;
		}
		auto colorRT = static_cast<RenderTarget*>(LogicResourceManager::getInstance()->getResource(ResourceType::RenderTarget, ResourceName::OpaqueRTName));
		if (colorRT == nullptr)
		{
			return false;
		}
		for (const auto& attachment : colorRT->getAttachments())
		{
			if (attachment.getResourceType() == FBOAttachmentResourceType::Texture && attachment.getAttachmentType() == FBOAttachmentType::Depth)
			{
				m_depthTex = attachment.getTexture();
			}
		}
		return m_depthTex != nullptr;
	}

	void onExecute() override
	{
		m_fullscreenMat->setProperty(ShaderPropertyNames::DepthBuffer, std::shared_ptr<MaterialProperty>(new MaterialTextureProperty(m_depthTex)));
		ForwardPassPostProcessing::onExecute();
	}

	void onDestroy() override
	{
		ForwardPassPostProcessing::onDestroy();
	}
private:
	Texture* m_depthTex;
	std::vector<glm::vec4> m_ssaoKernel;
};