#pragma once
#include <TextureEnums.h>
#include <GLMath.h>
#include <vector>
struct AttachmentSetting
{
	AttachmentSetting(TextureInternalFormat internalFormat, TexturePerChannelSize perChannelSize, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, TextureFilterMode textureFilter, bool enableSample, int levels, glm::vec4 borderColor) :
		m_internalFormat(internalFormat), m_wrapModeS(wrapModeS), m_wrapModeT(wrapModeT), m_textureFilter(textureFilter), m_perChannelSize(perChannelSize), m_enableSample(enableSample), m_levels(levels), m_borderColor(borderColor) {}
	AttachmentSetting() :AttachmentSetting(TextureInternalFormat::None, TexturePerChannelSize::UNSIGNED_BYTE, TextureWrapMode::ClampEdge, TextureWrapMode::ClampEdge, TextureFilterMode::Linear_Mipmap_Linear, false, 1, glm::vec4(0.0f)) {}

	TextureInternalFormat m_internalFormat;
	TextureWrapMode m_wrapModeS;
	TextureWrapMode m_wrapModeT;
	TextureFilterMode m_textureFilter;
	TexturePerChannelSize m_perChannelSize;
	bool m_enableSample;
	int m_levels;

	glm::vec4 m_borderColor;
};

struct RenderTargetDescriptor
{
	RenderTargetDescriptor(int width, int height, std::vector<AttachmentSetting> colorAttachmentSettings,
		AttachmentSetting depthAttachmentSetting, AttachmentSetting stencilAttachmentSetting)
		:m_width(width), m_height(height), m_colorAttachmentSettings(colorAttachmentSettings), m_depthAttachmentSetting(depthAttachmentSetting), m_stencilAttachmentSetting(stencilAttachmentSetting)
	{}
	RenderTargetDescriptor() :RenderTargetDescriptor(0, 0, std::vector<AttachmentSetting>(), AttachmentSetting(), AttachmentSetting()) {}

	int m_width;
	int m_height;

	std::vector<AttachmentSetting> m_colorAttachmentSettings;

	AttachmentSetting m_depthAttachmentSetting;
	AttachmentSetting m_stencilAttachmentSetting;

	TextureInternalFormat m_colorInternalFormat;
	TextureInternalFormat m_depthInternalFormat;
	TextureInternalFormat m_stencilInternalFormat;
	TexturePerChannelSize m_perChannelSize;
	TextureWrapMode m_wrapModeS;
	TextureWrapMode m_wrapModeT;
	TextureFilterMode m_textureFilter;
	TextureFilterMode m_depthTextureFilter;
};
