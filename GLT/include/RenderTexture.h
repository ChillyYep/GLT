#pragma once
#include <Object.h>
#include <iostream>
#include <Texture.h>
#include <RenderTargetTextureBase.h>
#include <LogicResourceManager.h>

class RenderTexture :public RenderTargetTextureBase
{
public:
	RenderTexture(int width, int height, TextureInternalFormat colorInternalFormat, TextureInternalFormat depthInternalFormat,
		TextureInternalFormat stencilInternalFormat = TextureInternalFormat::None,
		TexturePerChannelSize perChannelSize = TexturePerChannelSize::UNSIGNED_BYTE,
		TextureWrapMode wrapModeS = TextureWrapMode::ClampEdge, TextureWrapMode wrapModeT = TextureWrapMode::ClampEdge,
		TextureFilterMode textureFilter = TextureFilterMode::Point_Mipmap_Point, TextureFilterMode depthTextureFilter = TextureFilterMode::Point_Mipmap_Point
		, glm::vec4 borderColor = glm::vec4(0.0f), glm::vec4 depthBorderColor = glm::vec4(0.0f));

	~RenderTexture() {}

	void setColorAttachmentSampleEnabled(bool enabled)
	{
		assert(!m_isCreated && "Please call function \"setColorAttachmentSampleEnabled\" before \"create\"!");
		if (m_descriptor.m_colorAttachmentSettings.size() > 0)
		{
			m_descriptor.m_colorAttachmentSettings[0].m_enableSample = enabled;
		}
	}
private:
};