#include "RenderTexture.h"
RenderTexture::RenderTexture(int width, int height, TextureInternalFormat colorInternalFormat, TextureInternalFormat depthInternalFormat,
	TextureInternalFormat stencilInternalFormat, TexturePerChannelSize perChannelSize, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, TextureFilterMode textureFilter,
	TextureFilterMode depthTextureFilter, glm::vec4 borderColor, glm::vec4 depthBorderColor)
{
	auto colorAttachmentSettings = std::vector<AttachmentSetting>();
	if (colorInternalFormat != TextureInternalFormat::None)
	{
		auto colorAttachmentSetting = AttachmentSetting(colorInternalFormat, perChannelSize, wrapModeS, wrapModeT, textureFilter, false, 1, borderColor);
		colorAttachmentSettings.push_back(colorAttachmentSetting);
	}
	auto depthAttachmentSetting = AttachmentSetting(depthInternalFormat, perChannelSize, TextureWrapMode::Border, TextureWrapMode::Border, depthTextureFilter, false, 1, depthBorderColor);
	auto stencilAttachmentSetting = AttachmentSetting(stencilInternalFormat, perChannelSize, TextureWrapMode::ClampEdge, TextureWrapMode::ClampEdge, TextureFilterMode::Point_Mipmap_Point, false, 1, glm::vec4(0.0f));
	m_descriptor = RenderTargetDescriptor(width, height, colorAttachmentSettings, depthAttachmentSetting, stencilAttachmentSetting);
}

