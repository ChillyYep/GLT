#include "Texture.h"

void Texture::release() {
	if (m_data != nullptr)
	{
		delete[] m_data;
		m_data = nullptr;
	}
}

Texture2D* Texture2D::getBlackTex2D() {
	return setSinglePixelTex2D(s_blackTex, 0, 0, 0, 255);
}

Texture2D* Texture2D::getWhiteTex2D() {
	return setSinglePixelTex2D(s_whiteTex, 255, 255, 255, 255);
}
Texture2D* Texture2D::getCelesteTex2D() {
	return setSinglePixelTex2D(s_celesteTex, 0, 255, 255, 255);
}

Texture2D* Texture2D::getGrayTex2D() {
	return setSinglePixelTex2D(s_grayTex, 120, 120, 120, 255);
}
Texture2D* Texture2D::setSinglePixelTex2D(Texture2D& tex, GLTUByte r, GLTUByte g, GLTUByte b, GLTUByte a)
{
	if (tex.m_data == nullptr)
	{
		//glm::vec4 black = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		tex.m_data = new GLTUByte[4]{ r,g,b,a };
		//memcpy(s_blackTex.m_data, glm::value_ptr(black), sizeof(black));
		tex.m_width = 1;
		tex.m_height = 1;
		tex.setExternalFormat(TextureExternalFormat::RGB);
		tex.setInternalFormat(TextureInternalFormat::RGBA8);
		tex.setLevels(1);
		tex.setPerChannelSize(TexturePerChannelSize::UNSIGNED_BYTE);
		tex.setWrapModeS(TextureWrapMode::Repeat);
		tex.setWrapModeT(TextureWrapMode::Repeat);
		tex.setTextureFilter(TextureFilterMode::Linear_Mipmap_Linear);
	}
	return &tex;
}
Texture2D Texture2D::s_whiteTex;
Texture2D Texture2D::s_blackTex;
Texture2D Texture2D::s_celesteTex;
Texture2D Texture2D::s_grayTex;