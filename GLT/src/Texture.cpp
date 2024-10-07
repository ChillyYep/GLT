#include "Texture.h"

void Texture::unload() {
	if (m_data != nullptr)
	{
		delete[] m_data;
		m_data = nullptr;
	}
}

void Texture2D::load(const char* filename) {
	assert(m_data == nullptr);
	int channels;
	// 加载后再拷贝，消除stb库影响
	GLTUByte* imageData = stbi_load(filename, &m_width, &m_height, &channels, 0);
	m_data = new GLTUByte[m_width * m_height * channels];
	memcpy(m_data, imageData, m_width * m_height * channels);
	stbi_image_free(imageData);
	imageData = nullptr;
	if (channels == 1)
	{
		m_colorExternalFormat = TextureExternalFormat::RED;
	}
	else if (channels == 2)
	{
		m_colorExternalFormat = TextureExternalFormat::RG;
	}
	else if (channels == 3)
	{
		m_colorExternalFormat = TextureExternalFormat::RGB;
	}
	else if (channels == 4)
	{
		m_colorExternalFormat = TextureExternalFormat::RGBA;
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