#pragma once
#include <TextureEnums.h>

struct RenderTargetDescriptor
{
	RenderTargetDescriptor(int width, int height, TextureInternalFormat colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat,
		RenderTextureDepthStencilType stencilInternalFormat,
		TexturePerChannelSize perChannelSize, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, TextureFilterMode textureFilter)
		:m_width(width), m_height(height), m_colorInternalFormat(colorInternalFormat), m_depthInternalFormat(depthInternalFormat), m_stencilInternalFormat(stencilInternalFormat),
		m_perChannelSize(perChannelSize), m_wrapModeS(wrapModeS), m_wrapModeT(wrapModeT), m_textureFilter(textureFilter)
	{}
	RenderTargetDescriptor() :RenderTargetDescriptor(0, 0, TextureInternalFormat::None, RenderTextureDepthStencilType::None,
		RenderTextureDepthStencilType::None, TexturePerChannelSize::UNSIGNED_BYTE, TextureWrapMode::ClampEdge, TextureWrapMode::ClampEdge, TextureFilterMode::Linear_Mipmap_Linear) {}
	
	int m_width;
	int m_height;
	TextureInternalFormat m_colorInternalFormat;
	RenderTextureDepthStencilType m_depthInternalFormat;
	RenderTextureDepthStencilType m_stencilInternalFormat;
	TexturePerChannelSize m_perChannelSize;
	TextureWrapMode m_wrapModeS;
	TextureWrapMode m_wrapModeT;
	TextureFilterMode m_textureFilter;
};
