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

enum class TextureFilter {
	Point_Mipmap_Point,
	Linear_Mipmap_Point,
	Point_Mipmap_Linear,
	Linear_Mipmap_Linear,
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

enum class ConstantBufferType
{
	None,
	PerFrame,
	PerCamera,
	PerPass,
	//ShaderGlobalType_PerObject
};

enum class RenderCommandType
{
	Invalid,
	Default,
	RequestRenderTexture,
	Draw
};

enum class TextureFormat {

};

enum class TextureFilterMode {

};