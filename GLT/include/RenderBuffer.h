#pragma once
#include <CommonDefine.h>
#include <Object.h>
#include <TextureEnums.h>

struct RenderBufferDesc {
	int m_width;
	int m_height;
	bool m_isDepthBuffer;
	TextureInternalFormat m_colorInternalFormat;
	RenderTextureDepthStencilType m_depthStencilType;
};

class RenderBuffer :public Object
{
public:
	RenderBuffer(RenderBufferDesc renderbufferDesc)
	{
		m_renderBufferDesc = renderbufferDesc;
	}
	RenderBuffer() :RenderBuffer(RenderBufferDesc()) {}
	~RenderBuffer() {}

	int getWidth() const { return m_renderBufferDesc.m_width; }
	int getHeight() const { return m_renderBufferDesc.m_height; }

	TextureInternalFormat getColorInternalFormat() { return m_renderBufferDesc.m_colorInternalFormat; }
	void setColorInternalFormat(TextureInternalFormat colorInternalFormat) { m_renderBufferDesc.m_colorInternalFormat = colorInternalFormat; }
	RenderTextureDepthStencilType getDepthStencilType() { return m_renderBufferDesc.m_depthStencilType; }
	void setDepthStencilType(RenderTextureDepthStencilType depthStencilType) { m_renderBufferDesc.m_depthStencilType = depthStencilType; }

	bool IsDepthBuffer() { return m_renderBufferDesc.m_isDepthBuffer; }
private:
	RenderBufferDesc m_renderBufferDesc;
};