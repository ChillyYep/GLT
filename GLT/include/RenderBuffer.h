#pragma once
#include <CommonDefine.h>
#include <GLCommon.h>
#include <Object.h>
#include <TextureEnums.h>

class RenderBuffer :public Object
{
public:
	RenderBuffer(int width, int height, bool isDepthBuffer)
	{
		reset(width, height, isDepthBuffer);
	}
	RenderBuffer() :RenderBuffer(0, 0, false) {}
	~RenderBuffer() {}

	void reset(int width, int height, bool isDepthBuffer)
	{
		m_width = width;
		m_height = height;
		m_isDepthBuffer = isDepthBuffer;
		m_colorInternalFormat = TextureInternalFormat::None;
		m_depthStencilType = RenderTextureDepthStencilType::None;
	}

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

	TextureInternalFormat getColorInternalFormat() { return m_colorInternalFormat; }
	void setColorInternalFormat(TextureInternalFormat colorInternalFormat) { m_colorInternalFormat = colorInternalFormat; }
	RenderTextureDepthStencilType getDepthStencilType() { return m_depthStencilType; }
	void setDepthStencilType(RenderTextureDepthStencilType depthStencilType) { m_depthStencilType = depthStencilType; }

	bool IsDepthBuffer() { return m_isDepthBuffer; }
private:
	int m_width;
	int m_height;
	bool m_isDepthBuffer;
	TextureInternalFormat m_colorInternalFormat;
	RenderTextureDepthStencilType m_depthStencilType;
};