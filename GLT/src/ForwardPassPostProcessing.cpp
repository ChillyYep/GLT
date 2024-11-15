#include "ForwardPassPostProcessing.h"
SubMesh* ForwardPassPostProcessing::s_fullscreenTriangle = nullptr;
int ForwardPassPostProcessing::s_postProcessingPassCount = 0;
RenderTexture* ForwardPassPostProcessing::s_uberPostRT = nullptr;

void ForwardPassPostProcessing::appointShader(std::string shaderName)
{
	m_fullscreenShader = std::shared_ptr<Shader>(new Shader(shaderName));

	m_fullscreenMat = new Material(m_fullscreenShader);

	updateMaterialProperties(m_fullscreenMat);
}

SubMesh* ForwardPassPostProcessing::createFullscreenTraingleMesh()
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

void ForwardPassPostProcessing::onDefine()
{
	m_renderStateBlock.m_colorState.m_cullMode = CullMode::Off;
	m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(true, true, true, true);

	m_renderStateBlock.m_depthState.m_writable = false;
	m_renderStateBlock.m_depthState.m_depthRange = glm::vec2(0, 1);
	m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Always;
}

void ForwardPassPostProcessing::onPrepare()
{
	PassBase::onPrepare();
	s_postProcessingPassCount++;

	if (s_fullscreenTriangle == nullptr)
	{
		s_fullscreenTriangle = createFullscreenTraingleMesh();
		LogicResourceManager::getInstance()->addResource(ResourceType::Mesh, s_fullscreenTriangle);
	}
	auto window = Window::getInstance();
	auto windowSize = window->getSize();

	if (s_uberPostRT == nullptr)
	{
		s_uberPostRT = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGBA8, TextureInternalFormat::None);
		s_uberPostRT->m_name = ResourceName::PostProcessingRTName;
		s_uberPostRT->setColorAttachmentSampleEnabled(true);
		s_uberPostRT->create();
	}
}

bool ForwardPassPostProcessing::isPrepared()
{
	if (m_fullscreenShader == nullptr || m_fullscreenMat == nullptr)
	{
		return false;
	}
	auto colorRT = static_cast<RenderTarget*>(LogicResourceManager::getInstance()->getResource(ResourceType::RenderTarget, ResourceName::OpaqueRTName));
	if (colorRT == nullptr)
	{
		return false;
	}

	auto attachments = colorRT->getAttachments();
	m_colorRTTexture = nullptr;
	for (const auto& attachment : attachments)
	{
		if (attachment.getResourceType() == FBOAttachmentResourceType::Texture && attachment.getAttachmentType() == FBOAttachmentType::Color)
		{
			m_colorRTTexture = attachment.getTexture();
			break;
		}
	}
	if (m_colorRTTexture == nullptr)
	{
		return false;
	}
	m_uberRTIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, s_uberPostRT->getRTInstanceId()));
	m_colorRTIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, colorRT->getInstanceId()));

	return m_uberRTIdentifier != nullptr && m_colorRTIdentifier != nullptr;
}

void ForwardPassPostProcessing::onExecute()
{
	if (m_colorRTTexture != nullptr && m_uberRTIdentifier != nullptr)
	{
		m_context->setRenderStateBlock(m_renderStateBlock);
		// 将该临时RT作为渲染对象
		m_cmdBuffer.setRenderTarget(m_uberRTIdentifier);
		// 后处理渲染
		m_fullscreenMat->setProperty(ShaderPropertyNames::MainTex, std::shared_ptr<MaterialProperty>(new MaterialTextureProperty(m_colorRTTexture, ResourceCommonRegisterIndices::MainTexRegisterIndex)));
		m_cmdBuffer.drawMesh(s_fullscreenTriangle, m_fullscreenMat, glm::identity<glm::mat4>());
		m_context->scheduleCommandBuffer(m_cmdBuffer);
		m_cmdBuffer.clear();
		m_context->submit();
		m_context->blitRT(m_uberRTIdentifier, m_colorRTIdentifier);
	}
}

void ForwardPassPostProcessing::onDestroy()
{
	s_postProcessingPassCount--;
	if (s_postProcessingPassCount == 0)
	{
		if (s_uberPostRT != nullptr)
		{
			s_uberPostRT->release();
			delete s_uberPostRT;
			s_uberPostRT = nullptr;
		}
		if (s_fullscreenTriangle != nullptr)
		{
			delete s_fullscreenTriangle;
			s_fullscreenTriangle = nullptr;
		}
	}

	if (m_fullscreenMat != nullptr)
	{
		delete m_fullscreenMat;
		m_fullscreenMat = nullptr;
	}
}