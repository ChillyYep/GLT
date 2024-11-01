#pragma once
#include <Texture.h>
enum class CubemapFace {
	Front,
	Left,
	Back,
	Right,
	Top,
	Bottom,
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
	}
	Texture2D* getFace(CubemapFace cubemapFace)
	{
		return m_faces[(int)cubemapFace];
	}
	__GET_SET_PROPERTY__(Height, GLTSizei, m_height)
		__GET_SET_PROPERTY__(WrapModeT, TextureWrapMode, m_wrapModeT)
private:
	const int CUBEMAP_FACENUM = 6;
	Texture2D** m_faces;
	GLTSizei m_height;
	TextureWrapMode m_wrapModeT;
};