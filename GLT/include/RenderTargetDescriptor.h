#pragma once
#include <TextureEnums.h>

struct RenderTargetDescriptor
{
	RenderTargetDescriptor(int width, int height, TextureInternalFormat colorInternalFormat, TextureInternalFormat depthInternalFormat,
		TextureInternalFormat stencilInternalFormat,
		TexturePerChannelSize perChannelSize, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, TextureFilterMode textureFilter, TextureFilterMode depthTextureFilter)
		:m_width(width), m_height(height), m_colorInternalFormat(colorInternalFormat), m_depthInternalFormat(depthInternalFormat), m_stencilInternalFormat(stencilInternalFormat),
		m_perChannelSize(perChannelSize), m_wrapModeS(wrapModeS), m_wrapModeT(wrapModeT), m_textureFilter(textureFilter), m_depthTextureFilter(depthTextureFilter)
	{}
	RenderTargetDescriptor() :RenderTargetDescriptor(0, 0, TextureInternalFormat::None, TextureInternalFormat::None,
		TextureInternalFormat::None, TexturePerChannelSize::UNSIGNED_BYTE, TextureWrapMode::ClampEdge, TextureWrapMode::ClampEdge, TextureFilterMode::Linear_Mipmap_Linear, TextureFilterMode::Linear_Mipmap_Linear) {}

	int m_width;
	int m_height;
	TextureInternalFormat m_colorInternalFormat;
	TextureInternalFormat m_depthInternalFormat;
	TextureInternalFormat m_stencilInternalFormat;
	TexturePerChannelSize m_perChannelSize;
	TextureWrapMode m_wrapModeS;
	TextureWrapMode m_wrapModeT;
	TextureFilterMode m_textureFilter;
	TextureFilterMode m_depthTextureFilter;
};
