#pragma once
#include "PassBase.h"
#include <PrimitiveUtils.h>
#include <GLMath.h>
#include <Material.h>
#include <RenderResourceManagment.h>
#include <LogicResourceManager.h>

class PostProcessPass :public PassBase
{
public:
	PostProcessPass() {}
	~PostProcessPass() {}

	void prepare() override
	{
		PassBase::prepare();
		PrimitiveUtils::createQuad();
	}

	Mesh* createFullscreenTraingleMesh()
	{
		glm::vec4 vertices[] = {
			glm::vec4(.0f,.0f,.0f,1.f),
			glm::vec4(.0f,2.f,.0f,1.f),
			glm::vec4(2.f,.0f,.0f,1.f)
		};
		GLTUInt16 indices[] = { 0,1,2 };

		m_fullscreenTriangle = new Mesh(3, 3);
		m_fullscreenTriangle->setVertices(vertices);
		m_fullscreenTriangle->setIndices(indices);

		m_fullscreenShader = std::shared_ptr<Shader>(new Shader("PostProcessing"));
		m_fullscreenMat = new Material(m_fullscreenShader);
	}

	void execute() override
	{
		auto colorRT = static_cast<RenderTarget*>(LogicResourceManager::getInstance()->getResource(ResourceType::RenderTarget, "OpaqueRT"));
		auto colorRTIdentifier = colorRT == nullptr ? nullptr : static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, colorRT->getInstanceId()));
		if (colorRTIdentifier != nullptr)
		{
			m_cmdBuffer.drawMesh(m_fullscreenTriangle, m_fullscreenMat, glm::identity<glm::mat4>());
			//m_fullscreenMat->setProperty(ShaderPropertyNames::MainTex, std::shared_ptr<MaterialProperty>(new MaterialTextureProperty(colorRT)));
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
		}
	}

	void destroy() override
	{
		if (m_fullscreenTriangle != nullptr)
		{
			delete m_fullscreenTriangle;
			m_fullscreenTriangle = nullptr;
		}
		if (m_fullscreenMat != nullptr)
		{
			delete m_fullscreenMat;
			m_fullscreenMat = nullptr;
		}
	}
private:
	Mesh* m_fullscreenTriangle;
	Material* m_fullscreenMat;
	std::shared_ptr<Shader> m_fullscreenShader;
};