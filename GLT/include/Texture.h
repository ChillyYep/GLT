#pragma once
#include <CommonDefine.h>
#include <Object.h>
#include <stb_image.h>
#include <TextureEnums.h>
#include <GLCommon.h>
#include <ResourceIdentifier.h>

class Texture :public Object
{
public:
	Texture(TextureType textureType) :m_textureType(textureType) {}
	~Texture() {}
	virtual void load(const char* filename) = 0;
	virtual void unload() = 0;

	inline GLTUByte* getData() { return m_data; }

	inline TextureType getTextureType() { return m_textureType; }
	__GET_SET_PROPERTY__(Levels, GLTSizei, m_levels)
		__GET_SET_PROPERTY__(Width, GLTSizei, m_width)
		__GET_SET_PROPERTY__(IsProxy, GLTBool, m_isProxy)
		__GET_SET_PROPERTY__(InternalFormat, TextureInternalFormat, m_internalFormat)
		__GET_SET_PROPERTY__(ExternalFormat, TextureExternalFormat, m_externalFormat)
		__GET_SET_PROPERTY__(PerChannelSize, TexturePerChannelSize, m_perChannelSize)
		__GET_SET_PROPERTY__(CanReadWrite, GLTBool, m_readWrite)
		__GET_SET_PROPERTY__(WrapModeS, TextureWrapMode, m_wrapModeS)
		__GET_SET_PROPERTY__(TextureFilter, TextureFilterMode, m_textureFilter)

protected:
	GLTUByte* m_data;

	GLTBool m_readWrite;

	TextureType m_textureType;
	GLTBool m_isProxy;
	GLTSizei m_levels;
	GLTSizei m_width;
	TextureInternalFormat m_internalFormat;
	TextureExternalFormat m_externalFormat = TextureExternalFormat::RED;
	TexturePerChannelSize m_perChannelSize;
	TextureWrapMode m_wrapModeS;
	TextureFilterMode m_textureFilter;
};
class Texture1D :public Texture
{
public:
	Texture1D() :Texture(TextureType::Texture1D) {}

};
class Texture2D :public Texture
{
public:
	Texture2D() :Texture(TextureType::Texture2D) {}
	void load(const char* filename) override {
		if (m_data != nullptr)
		{
			unload();
		}
		int channels;
		m_data = stbi_load(filename, &m_width, &m_height, &channels, 0);
		if (channels == 1)
		{
			m_externalFormat = TextureExternalFormat::RED;
		}
		else if (channels == 2)
		{
			m_externalFormat = TextureExternalFormat::RG;
		}
		else if (channels == 3)
		{
			m_externalFormat = TextureExternalFormat::RGB;
		}
		else if (channels == 4)
		{
			m_externalFormat = TextureExternalFormat::RGBA;
		}
	}
	void unload() override {
		if (m_data != nullptr)
		{
			stbi_image_free(m_data);
			m_data = nullptr;
		}
	}
	__GET_SET_PROPERTY__(Height, GLTSizei, m_height)
		__GET_SET_PROPERTY__(WrapModeT, TextureWrapMode, m_wrapModeT)
protected:
	GLTSizei m_height;
	TextureWrapMode m_wrapModeT;
};

class Texture3D :public Texture
{
public:
	Texture3D() :Texture(TextureType::Texture3D) {}
	__GET_SET_PROPERTY__(Height, GLTSizei, m_height)
		__GET_SET_PROPERTY__(Depth, GLTSizei, m_depth)
		__GET_SET_PROPERTY__(WrapModeP, TextureWrapMode, m_wrapModeP)
		__GET_SET_PROPERTY__(WrapModeT, TextureWrapMode, m_wrapModeT)
protected:
	GLTSizei m_height;
	GLTSizei m_depth;
	TextureWrapMode m_wrapModeT;
	TextureWrapMode m_wrapModeP;
};