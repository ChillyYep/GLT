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
		m_colorExternalFormat(TextureExternalFormat::RED), m_isProxy(false), m_levels(0), m_width(0),
		m_colorInternalFormat(TextureInternalFormat::None), m_perChannelSize(TexturePerChannelSize::UNSIGNED_BYTE), m_wrapModeS(TextureWrapMode::ClampEdge),
		m_textureFilter(TextureFilterMode::Linear_Mipmap_Linear), m_dirty(false)
	{}
	virtual ~Texture() { unload(); }
	virtual void load(const char* filename) {}
	void unload();

	inline GLTUByte* getData() { return m_data; }

	inline void setData(GLTUByte* data, int size = 0)
	{
		unload();
		if (data == nullptr)
		{
			return;
		}
		setDirty(true);
		m_data = new GLTUByte[size];
		memcpy(m_data, data, size);
	}

	inline TextureType getTextureType() { return m_textureType; }

	inline static TextureExternalFormat getDefaultTextureExternalFormat(TextureInternalFormat internalFormat)
	{
		switch (internalFormat)
		{
		case TextureInternalFormat::None:
			break;
		case TextureInternalFormat::R8:
		case TextureInternalFormat::R16:
		case TextureInternalFormat::R16F:
		case TextureInternalFormat::R32F:
		case TextureInternalFormat::R8_SNORM:
		case TextureInternalFormat::R16_SNORM:
			return TextureExternalFormat::RED;
		case TextureInternalFormat::RG8:
		case TextureInternalFormat::RG16:
		case TextureInternalFormat::RG16F:
		case TextureInternalFormat::RG32F:
		case TextureInternalFormat::RG8_SNORM:
		case TextureInternalFormat::RG16_SNORM:
			return TextureExternalFormat::RG;
		case TextureInternalFormat::RGB8:
		case TextureInternalFormat::RGB16:
		case TextureInternalFormat::R3G3B2:
		case TextureInternalFormat::SRGB8:
		case TextureInternalFormat::RGB16F:
		case TextureInternalFormat::RGB32F:
		case TextureInternalFormat::R11F_G11F_B10F:
		case TextureInternalFormat::RGB9_E5:
		case TextureInternalFormat::RGB8_SNORM:
		case TextureInternalFormat::RGB16_SNORM:
			return TextureExternalFormat::RGB;
		case TextureInternalFormat::RGBA8:
		case TextureInternalFormat::RGBA16:
		case TextureInternalFormat::SRGBA8:
		case TextureInternalFormat::RGBA4:
		case TextureInternalFormat::RGBA16F:
		case TextureInternalFormat::RGBA32F:
		case TextureInternalFormat::RGBA8_SNORM:
		case TextureInternalFormat::RGBA16_SNORM:
			return TextureExternalFormat::RGBA;
		case TextureInternalFormat::Depth8:
		case TextureInternalFormat::Depth16:
		case TextureInternalFormat::Depth24:
		case TextureInternalFormat::Depth32:
		case TextureInternalFormat::Depth32F:
		case TextureInternalFormat::Stencil0:
		case TextureInternalFormat::Stencil1:
		case TextureInternalFormat::Stencil4:
		case TextureInternalFormat::Stencil8:
		case TextureInternalFormat::Stencil16:
		case TextureInternalFormat::Depth_Stencil:
		default:
			break;
		}
		return TextureExternalFormat::RGB;
	}

	__GET_SET_PROPERTY__(Dirty, bool, m_dirty)
		__GET_SET_PROPERTY__(Levels, GLTSizei, m_levels)
		__GET_SET_PROPERTY__(Width, GLTSizei, m_width)
		__GET_SET_PROPERTY__(IsProxy, GLTBool, m_isProxy)
		__GET_SET_PROPERTY__(InternalFormat, TextureInternalFormat, m_colorInternalFormat)
		__GET_SET_PROPERTY__(ExternalFormat, TextureExternalFormat, m_colorExternalFormat)
		__GET_SET_PROPERTY__(PerChannelSize, TexturePerChannelSize, m_perChannelSize)
		__GET_SET_PROPERTY__(CanReadWrite, GLTBool, m_readWrite)
		__GET_SET_PROPERTY__(WrapModeS, TextureWrapMode, m_wrapModeS)
		__GET_SET_PROPERTY__(TextureFilter, TextureFilterMode, m_textureFilter)

protected:
	GLTUByte* m_data;

	bool m_dirty;

	GLTBool m_readWrite;

	TextureType m_textureType;
	GLTBool m_isProxy;
	GLTSizei m_levels;
	GLTSizei m_width;
	TextureInternalFormat m_colorInternalFormat;
	TextureExternalFormat m_colorExternalFormat = TextureExternalFormat::RED;
	TextureExternalFormat m_depthExternalFormat = TextureExternalFormat::RED;
	TextureExternalFormat m_stencilExternalFormat = TextureExternalFormat::RED;
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
		:Texture(TextureType::Texture2D), m_height(0), m_wrapModeT(TextureWrapMode::ClampEdge)
	{}
	~Texture2D() {}
	void load(const char* filename) override;
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