#pragma once
#include <ForwardPassPostProcessing.h>
#include <random>

class ForwardPassScreenSpaceAmbientOcclusion :public ForwardPassPostProcessing
{
public:
	ForwardPassScreenSpaceAmbientOcclusion() {}
	~ForwardPassScreenSpaceAmbientOcclusion() {}

	bool isExecutable() override { return true; }

	void onDefine() override
	{
		ForwardPassPostProcessing::onDefine();

		// �������������Χ0.0~1.0
		std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
		std::default_random_engine generator;
		const int randomSampleCount = 64;
		m_ssaoKernel.resize(randomSampleCount);
		for (unsigned int i = 0; i < randomSampleCount; ++i)
		{
			// ��������㣬�ֲ���z�����������
			glm::vec4 sample(randomFloats(generator) * 2.0f - 1.0f,
				randomFloats(generator) * 2.0f - 1.0f,
				randomFloats(generator), 0.0f);
			sample = glm::normalize(sample);
			float scale = float(i) / 64.0f;
			// ʹ������ֲ�������Ƭ�α���
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
		return true;
	}

	void onExecute() override
	{
		ForwardPassPostProcessing::onExecute();
	}

	void onDestroy() override
	{
		ForwardPassPostProcessing::onDestroy();
	}
private:
	std::vector<glm::vec4> m_ssaoKernel;
};