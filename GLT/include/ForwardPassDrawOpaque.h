#pragma once
#include <PassBase.h>
#include <RenderTexture.h>
#include <RenderResourceManagment.h>
#include <ResourceNames.h>

class ForwardPassDrawOpaque :public PassBase
{
public:
	ForwardPassDrawOpaque() :PassBase() {}
	~ForwardPassDrawOpaque() {}
private:
	void onDefine() override
	{
		m_renderStateBlock.m_colorState.m_cullMode = CullMode::Back;
		m_renderStateBlock.m_colorState.m_rgbaWritable = glm::bvec4(true, true, true, true);
		m_renderStateBlock.m_depthState.m_depthRange = glm::vec2(0, 1);
		m_renderStateBlock.m_depthState.m_writable = true;
		m_renderStateBlock.m_depthState.m_compareFunc = CompareFunction::Less;

		m_filterSettings.m_renderType = RenderType::Opaque;
		m_drawSettings.m_sortType = SortType::Near2Far;
	}

	void onPrepare() override
	{
		PassBase::onPrepare();
		// prepareResources
		auto window = Window::getInstance();
		auto windowSize = window->getSize();
		m_colorRT = new RenderTexture(windowSize.x, windowSize.y, TextureInternalFormat::RGBA8, TextureInternalFormat::Depth16,
			TextureInternalFormat::None);
		m_colorRT->m_name = ResourceName::OpaqueRTName;
		m_colorRT->setColorAttachmentSampleEnabled(true);
		m_colorRT->setDepthAttachmentSampleEnabled(true);
		m_colorRT->create();
	}

	bool isPrepared() override
	{
		m_colorRTIdentifier = static_cast<RenderTargetIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::RenderTarget, m_colorRT->getRTInstanceId()));
		auto shadowMapRT = static_cast<RenderTarget*>(LogicResourceManager::getInstance()->getResource(ResourceType::RenderTarget, m_renderData->m_shadowData.m_shadowMapRTName));
		if (shadowMapRT == nullptr)
		{
			return false;
		}
		Texture2D* shadowMapTexture = nullptr;
		auto attachments = shadowMapRT->getAttachments();
		for (const auto& attachment : attachments)
		{
			if (attachment.getAttachmentType() == FBOAttachmentType::Depth && attachment.getResourceType() == FBOAttachmentResourceType::Texture)
			{
				shadowMapTexture = static_cast<Texture2D*>(attachment.getTexture());
				break;
			}
		}
		if (shadowMapTexture == nullptr)
		{
			return false;
		}
		m_shadowMapTextureIdentifier = static_cast<TextureResourceIdentifier*>(RenderResourceManagement::getInstance()->getResourceIdentifier(ResourceType::Texture, shadowMapTexture->getInstanceId()));
		return m_colorRTIdentifier != nullptr && m_shadowMapTextureIdentifier != nullptr;
	}

	void onDestroy() override
	{
		m_colorRT->release();
		delete m_colorRT;
		m_colorRT = nullptr;
	}
	bool isExecutable() override { return true; }

	void onExecute() override
	{
		m_context->setRenderStateBlock(m_renderStateBlock);
		// 如果多相机绘制，则相机会在同一帧发生变化，所以需要及时更新
		m_drawSettings.m_cameraPos = m_renderData->m_cameraDatas[m_renderData->m_curRenderingCameraIndex].m_worldPos;
		if (m_colorRTIdentifier != nullptr && m_shadowMapTextureIdentifier != nullptr)
		{
			m_cmdBuffer.setGlobalTextureResource(ShaderPropertyNames::ShadowMapTex, m_shadowMapTextureIdentifier);

			m_cmdBuffer.setRenderTarget(m_colorRTIdentifier);
			m_cmdBuffer.clearColor(0.0f, 0.0f, 0.0f, 0.0f);
			m_context->scheduleCommandBuffer(m_cmdBuffer);
			m_cmdBuffer.clear();
			m_context->submit();
			m_context->drawRenderers(m_filterSettings, m_drawSettings);
		}
	}

	RenderTargetIdentifier* m_colorRTIdentifier;
	TextureResourceIdentifier* m_shadowMapTextureIdentifier;

	FilterSettings m_filterSettings;
	DrawSettings m_drawSettings;
	RenderStateBlock m_renderStateBlock;
	RenderTexture* m_colorRT;

};