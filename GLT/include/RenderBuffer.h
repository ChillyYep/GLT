#pragma once
#include <CommonDefine.h>
#include <Object.h>
#include <TextureEnums.h>

struct RenderBufferDescriptor {
	int m_width;
	int m_height;
	bool m_isDepthBuffer;
	TextureInternalFormat m_internalFormat;
};

class RenderBuffer :public Object
{
public:
	RenderBuffer(RenderBufferDescriptor renderbufferDesc)
	{
		m_renderBufferDesc = renderbufferDesc;
	}
	RenderBuffer() :RenderBuffer(RenderBufferDescriptor()) {}
	~RenderBuffer() {}

	int getWidth() const { return m_renderBufferDesc.m_width; }
	int getHeight() const { return m_renderBufferDesc.m_height; }

	TextureInternalFormat getInternalFormat() { return m_renderBufferDesc.m_internalFormat; }
	void setInternalFormat(TextureInternalFormat colorInternalFormat) { m_renderBufferDesc.m_internalFormat = colorInternalFormat; }

	bool IsDepthBuffer() { return m_renderBufferDesc.m_isDepthBuffer; }
private:
	RenderBufferDescriptor m_renderBufferDesc;
};