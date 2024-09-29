#pragma once
enum class TextureType {
	Texture1D,
	Texture1DArray,
	Texture2D,
	Texture2DArray,
	Texture2DMultiSample,
	Texture2DMultiSampleArray,
	Texture3D,
	CubeMap,
	CubeMapArray,
	Buffer,
};

enum class TextureWrapMode {
	ClampEdge,
	Border,
	Repeat,
	MirroredRepeat
};

enum class TextureFilterMode {
	Point_Mipmap_Point,
	Linear_Mipmap_Point,
	Point_Mipmap_Linear,
	Linear_Mipmap_Linear,
};

enum class TextureInternalFormat {
	None,
	R8,
	RG8,
	RGB8,
	RGBA8,
	R16,
	RG16,
	RGB16,
	RGBA16,
	R3G3B2,
	SRGB8,
	SRGBA8,
	RGBA4,
	R16F,
	R32F,
	RG16F,
	RG32F,
	RGB16F,
	RGB32F,
	RGBA16F,
	RGBA32F,
	R11F_G11F_B10F,
	RGB9_E5,
	R8_SNORM,
	R16_SNORM,
	RG8_SNORM,
	RG16_SNORM,
	RGB8_SNORM,
	RGB16_SNORM,
	RGBA8_SNORM,
	RGBA16_SNORM,
};

enum class TextureExternalFormat {
	RED,
	RG,
	RGB,
	RGBA
};

enum class ColorChannel
{
	R = 1,
	G = 1 << 1,
	B = 1 << 2,
	A = 1 << 3
};

enum class TexturePerChannelSize {
	UNSIGNED_BYTE,
};

enum class RenderTextureDepthStencilType {
	None,
	Depth8,
	Depth16,
	Depth24,
	Depth32,
	Depth32F,
	Stencil0,
	Stencil1,
	Stencil4,
	Stencil8,
	Stencil16,
	Depth_Stencil,
};
