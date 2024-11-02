#pragma once
#include <Texture.h>
enum class CubemapFace {
	Right,
	Left,
	Top,
	Bottom,
	Front,
	Back,
};
class Cubemap :public Texture
{
public:
	Cubemap() :Texture(TextureType::CubeMap)
	{
		m_faces = new Texture2D * [CUBEMAP_FACENUM];
	}
	~Cubemap()
	{
		delete[] m_faces;
		m_faces = nullptr;
	}
	void setFace(CubemapFace cubemapFace, Texture2D* face)
	{
		m_faces[(int)cubemapFace] = face;
		if (face != nullptr)
		{
			face->setInternalFormat(m_colorInternalFormat);
			face->setLevels(m_levels);
			face->setPerChannelSize(m_perChannelSize);
			face->setTextureFilter(m_textureFilter);
		}
	}
	Texture2D* getFace(CubemapFace cubemapFace)
	{
		return m_faces[(int)cubemapFace];
	}
	static const int CUBEMAP_FACENUM;
	__GET_SET_PROPERTY__(Height, GLTSizei, m_height)
		__GET_SET_PROPERTY__(WrapModeT, TextureWrapMode, m_wrapModeT)
		__GET_SET_PROPERTY__(WrapModeR, TextureWrapMode, m_wrapModeR)
private:
	Texture2D** m_faces;
	GLTSizei m_height;
	TextureWrapMode m_wrapModeT;
	TextureWrapMode m_wrapModeR;
};