#pragma once
#include <CommonDefine.h>
#include <Object.h>
#include <stb_image.h>
#include <TextureEnums.h>
#include <GLCommon.h>
#include <GLMath.h>
#include <ResourceIdentifier.h>

class Texture :public Object
{
public:
	Texture(TextureType textureType)
		:m_textureType(textureType), m_data(nullptr), m_readWrite(false),
		m_externalFormat(TextureExternalFormat::RED), m_isProxy(false), m_levels(0), m_width(0),
		m_colorInternalFormat(TextureInternalFormat::None), m_perChannelSize(TexturePerChannelSize::UNSIGNED_BYTE), m_wrapModeS(TextureWrapMode::ClampEdge),
		m_textureFilter(TextureFilterMode::Linear_Mipmap_Linear)
	{}
	virtual ~Texture() {}
	virtual void load(const char* filename) {}
	virtual void unload() {}

	inline GLTUByte* getData() { return m_data; }

	inline void setData(GLTUByte* data) { m_data = data; }

	inline TextureType getTextureType() { return m_textureType; }

	__GET_SET_PROPERTY__(Levels, GLTSizei, m_levels)
		__GET_SET_PROPERTY__(Width, GLTSizei, m_width)
		__GET_SET_PROPERTY__(IsProxy, GLTBool, m_isProxy)
		__GET_SET_PROPERTY__(InternalFormat, TextureInternalFormat, m_colorInternalFormat)
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
	TextureInternalFormat m_colorInternalFormat;
	TextureExternalFormat m_externalFormat = TextureExternalFormat::RED;
	TexturePerChannelSize m_perChannelSize;
	TextureWrapMode m_wrapModeS;
	TextureFilterMode m_textureFilter;
};
class Texture1D :public Texture
{
public:
	Texture1D() :Texture(TextureType::Texture1D) {}
	~Texture1D() {}
private:
};
class Texture2D :public Texture
{
public:
	Texture2D()
		:Texture(TextureType::Texture2D), m_loaded(false), m_height(0), m_wrapModeT(TextureWrapMode::ClampEdge)
	{}
	~Texture2D()
	{
		if (!m_loaded && m_data != nullptr)
		{
			delete[] m_data;
			m_data = nullptr;
		}
	}
	void load(const char* filename) override;
	void unload() override;
	/// <summary>
	/// 天青色
	/// </summary>
	/// <returns></returns>
	static Texture2D* getCelesteTex2D();

	/// <summary>
	/// 灰色
	/// </summary>
	/// <returns></returns>
	static Texture2D* getGrayTex2D();

	/// <summary>
	/// 黑色
	/// </summary>
	/// <returns></returns>
	static Texture2D* getBlackTex2D();

	/// <summary>
	/// 白色
	/// </summary>
	/// <returns></returns>
	static Texture2D* getWhiteTex2D();
	/// <summary>
	/// 设置单像素纹理
	/// </summary>
	/// <param name="tex"></param>
	/// <param name="r"></param>
	/// <param name="g"></param>
	/// <param name="b"></param>
	/// <param name="a"></param>
	/// <returns></returns>
	static Texture2D* setSinglePixelTex2D(Texture2D& tex, GLTUByte r, GLTUByte g, GLTUByte b, GLTUByte a);
	__GET_SET_PROPERTY__(Height, GLTSizei, m_height)
		__GET_SET_PROPERTY__(WrapModeT, TextureWrapMode, m_wrapModeT)
protected:

	bool m_loaded;
	GLTSizei m_height;
	TextureWrapMode m_wrapModeT;

	static Texture2D s_celesteTex;
	static Texture2D s_grayTex;
	static Texture2D s_whiteTex;
	static Texture2D s_blackTex;
};

class Texture3D :public Texture
{
public:
	Texture3D() :m_height(0), m_depth(0), m_wrapModeT(TextureWrapMode::ClampEdge), m_wrapModeP(TextureWrapMode::ClampEdge), Texture(TextureType::Texture3D) {}
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