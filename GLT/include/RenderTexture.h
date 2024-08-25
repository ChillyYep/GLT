#pragma once
#include <Object.h>
#include <Window.h>
#include <iostream>
#include <Texture.h>
#include <RenderTarget.h>
#include <RenderResourceManager.h>

class RenderTexture :public Texture2D
{
public:
	RenderTexture(int width, int height, TextureInternalFormat colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat,
		RenderTextureDepthStencilType stencilInternalFormat = RenderTextureDepthStencilType::None,
		TexturePerChannelSize perChannelSize = TexturePerChannelSize::UNSIGNED_BYTE,
		TextureWrapMode wrapModeS = TextureWrapMode::ClampEdge, TextureWrapMode wrapModeT = TextureWrapMode::ClampEdge,
		TextureFilterMode textureFilter = TextureFilterMode::Linear_Mipmap_Linear)
		:Texture2D(), m_renderTarget(nullptr), m_colorRB(0, 0, false), m_depthRB(0, 0, true), m_stencilRB(0, 0, true)
	{
		m_readWrite = false;
		m_isProxy = false;
		m_levels = 1;
		m_width = width;
		m_height = height;
		m_colorInternalFormat = colorInternalFormat;
		m_depthInternalFormat = depthInternalFormat;
		m_stencilInternalFormat = stencilInternalFormat;
		m_externalFormat = TextureExternalFormat::RGBA;
		m_perChannelSize = perChannelSize;
		m_wrapModeS = wrapModeS;
		m_wrapModeT = wrapModeT;
		m_textureFilter = textureFilter;
	}

	~RenderTexture() {}

	void create()
	{
		if (m_isCreated)
		{
			return;
		}
		m_isCreated = true;
		if (m_renderTarget != nullptr)
		{
			return;
		}
		m_renderTarget = std::shared_ptr<RenderTarget>(new RenderTarget(m_width, m_height, m_colorInternalFormat, m_depthInternalFormat, m_stencilInternalFormat, m_perChannelSize, m_wrapModeS, m_wrapModeT, m_textureFilter));
		if (m_colorInternalFormat != TextureInternalFormat::None)
		{
			m_colorRB.reset(m_width, m_height, false);
			m_colorRB.setColorInternalFormat(m_colorInternalFormat);
			m_renderTarget->addAttachment(AttachmentEntityWrapper(&m_colorRB, FBOAttachmentType::Color));
		}
		if (m_depthInternalFormat != RenderTextureDepthStencilType::None)
		{
			m_depthRB.reset(m_width, m_height, true);
			m_depthRB.setDepthStencilType(m_depthInternalFormat);
			m_renderTarget->addAttachment(AttachmentEntityWrapper(&m_depthRB,
				m_depthInternalFormat == RenderTextureDepthStencilType::Depth_Stencil ?
				FBOAttachmentType::DepthStencil : FBOAttachmentType::Depth));
		}
		if (m_stencilInternalFormat != RenderTextureDepthStencilType::None)
		{
			m_stencilRB.reset(m_width, m_height, true);
			m_stencilRB.setDepthStencilType(m_stencilInternalFormat);
			m_renderTarget->addAttachment(AttachmentEntityWrapper(&m_stencilRB, FBOAttachmentType::Stencil));
		}
		auto& rtManagementCentre = RenderResourceManager::getInstance()->getRenderTargetManagementCentre();
		rtManagementCentre.add(m_renderTarget);
	}

	void release()
	{
		if (m_renderTarget == nullptr)
		{
			return;
		}

		m_colorRB.reset(0, 0, false);
		m_depthRB.reset(0, 0, true);
		m_stencilRB.reset(0, 0, true);
		auto& rtManagementCentre = RenderResourceManager::getInstance()->getRenderTargetManagementCentre();
		rtManagementCentre.remove(m_renderTarget);
		m_renderTarget = nullptr;
		m_isCreated = false;
	}

private:
	bool m_isCreated = false;
	RenderTextureDepthStencilType m_depthInternalFormat;
	RenderTextureDepthStencilType m_stencilInternalFormat;
	std::shared_ptr<RenderTarget> m_renderTarget;

	RenderBuffer m_colorRB;
	RenderBuffer m_depthRB;
	RenderBuffer m_stencilRB;
};