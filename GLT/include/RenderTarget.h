#pragma once
#include <Object.h>
#include <CommonDefine.h>
#include <TextureEnums.h>

enum class RenderTextureDepthStencilType {
	None,
	Depth8,
	Depth16,
	Depth24,
	Depth32,
	Depth32F,
	Stencil0,
	Stencil1,
	Stencil4,
	Stencil8,
	Stencil16,
	Depth_Stencil,
};

struct RenderTargetDescriptor
{
	RenderTargetDescriptor(int width, int height, TextureInternalFormat colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat, RenderTextureDepthStencilType stencilInternalFormat = RenderTextureDepthStencilType::None)
		:m_width(width), m_height(height), m_colorInternalFormat(colorInternalFormat), m_depthInternalFormat(depthInternalFormat), m_stencilInternalFormat(stencilInternalFormat) {}

	int m_width;
	int m_height;
	TextureInternalFormat m_colorInternalFormat;
	RenderTextureDepthStencilType m_depthInternalFormat;
	RenderTextureDepthStencilType m_stencilInternalFormat;
};

class RenderTarget :public Object
{
public:
	RenderTarget(int width, int height, TextureInternalFormat colorInternalFormat, RenderTextureDepthStencilType depthInternalFormat, RenderTextureDepthStencilType stencilInternalFormat = RenderTextureDepthStencilType::None)
		:m_renderTextureDescriptor(RenderTargetDescriptor(width, height, colorInternalFormat, depthInternalFormat, stencilInternalFormat)) {}
	~RenderTarget() {}

	virtual void create(bool immediately = true) = 0;
	virtual void release(bool immediately = true) = 0;

	__GET_SET_PROPERTY__(RenderTargetDescriptor, RenderTargetDescriptor, m_renderTextureDescriptor)
protected:

	RenderTargetDescriptor m_renderTextureDescriptor;
};