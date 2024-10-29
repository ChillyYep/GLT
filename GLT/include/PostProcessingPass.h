#pragma once
#include "PassBase.h"
#include <PrimitiveUtils.h>
#include <GLMath.h>
#include <Material.h>
#include <RenderResourceManagment.h>
#include <LogicResourceManager.h>
#include <ResourceNames.h>

class PostProcessingPass :public PassBase
{
public:
	PostProcessingPass() {}
	~PostProcessingPass() {}

	bool isExecutable() override { return false; }

	void prepare() override
	{
		PassBase::prepare();

		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Off;
		m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(true, true, true, true);

		m_renderStateBlock.m_depthState.m_writable = false;
		m_renderStateBlock.m_depthState.m_depthRange = glm::vec2(0, 1);
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Always;


		m_fullscreenTriangle = createFullscreenTraingleMesh();

		LogicResourceManager::getInstance()->addResource(ResourceType::Mesh, m_fullscreenTriangle);

		m_fullscreenShader = std::shared_ptr<Shader>(new Shader("postProcessing"));

		m_fullscreenMat = new Material(m_fullscreenShader);

		auto window = Window::getInstance();
		auto windowSize = window->getSize();
		m_uberPostRT = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGBA8, TextureInternalFormat::None,
			TextureInternalFormat::None);
		m_uberPostRT->m_name = ResourceName::PostProcessingRTName;
		m_uberPostRT->setColorAttachmentSampleEnabled(true);
		m_uberPostRT->create();
	}

	SubMesh* createFullscreenTraingleMesh()
	{
		glm::vec4 vertices[] = {
			glm::vec4(-1.f,-1.f,.0f,1.f),
			glm::vec4(3.0f,-1.f,.0f,1.f),
			glm::vec4(-1.f,3.0f,.0f,1.f)
		};
		GLTUInt16 indices[] = { 0,1,2 };

		SubMesh* fullscreenTriangle = new SubMesh(3, 3);
		fullscreenTriangle->setVertices(vertices);
		fullscreenTriangle->setIndices(indices);

		return fullscreenTriangle;
	}

	void execute() override
	{
		auto colorRT = static_cast<RenderTarget*>(LogicResourceManager::getInstance()->getResource(ResourceType::RenderTarget, ResourceName::OpaqueRTName));
		if (colorRT == nullptr)
		{
			return;
		}
		auto attachments = colorRT->getAttachments();
		Texture* texture = nullptr;
		for (const auto& attachment : attachments)
		{
			if (attachment.getResourceType() == FBOAttachmentResourceType::Texture && attachment.getAttachmentType() == FBOAttachmentType::Color)
			{
				texture = attachment.getTexture();
				break;
			}
		}

		auto uberPostRTIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_uberPostRT->getRTInstanceId()));
		auto colorRTIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, colorRT->getInstanceId()));
		if (texture != nullptr && uberPostRTIdentifier != nullptr)
		{
			m_context->setRenderStateBlock(m_renderStateBlock);
			// 将该临时RT作为渲染对象
			m_cmdBuffer.setRenderTarget(uberPostRTIdentifier);
			// 后处理渲染
			m_fullscreenMat->setProperty(ShaderPropertyNames::MainTex, std::shared_ptr<MaterialProperty>(new MaterialTextureProperty(texture)));
			m_cmdBuffer.drawMesh(m_fullscreenTriangle, m_fullscreenMat, glm::identity<glm::mat4>());
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
			// 将渲染结果传回
			//m_context->blitRT(uberPostRTIdentifier, colorRTIdentifier, FBOAttachmentType::Color);
			// 重新将原RT作为渲染对象
		/*	m_cmdBuffer.setRenderTarget(colorRTIdentifier);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();*/
		}
	}

	void destroy() override
	{
		if (m_uberPostRT != nullptr)
		{
			m_uberPostRT->release();
			delete m_uberPostRT;
			m_uberPostRT = nullptr;
		}
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
	SubMesh* m_fullscreenTriangle;
	Material* m_fullscreenMat;
	RenderTexture* m_uberPostRT;

	std::shared_ptr<Shader> m_fullscreenShader;

	RenderStateBlock m_renderStateBlock;
};