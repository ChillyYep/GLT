#pragma once
enum TextureType {
	TextureType_Texture1D,
	TextureType_Texture1DArray,
	TextureType_Texture2D,
	TextureType_Texture2DArray,
	TextureType_Texture2DMultiSample,
	TextureType_Texture2DMultiSampleArray,
	TextureType_Texture3D,
	TextureType_CubeMap,
	TextureType_CubeMapArray,
	TextureType_Buffer,
};

enum TextureWrapMode {
	TextureWrapMode_ClampEdge,
	TextureWrapMode_Border,
	TextureWrapMode_Repeat,
	TextureWrapMode_MirroredRepeat
};

enum TextureFilter {
	TextureFilter_Point_Mipmap_Point,
	TextureFilter_Linear_Mipmap_Point,
	TextureFilter_Point_Mipmap_Linear,
	TextureFilter_Linear_Mipmap_Linear,
};

enum RenderTextureDepthStencilType {
	RenderTextureDepthStencilType_None,
	RenderTextureDepthStencilType_Depth8,
	RenderTextureDepthStencilType_Depth16,
	RenderTextureDepthStencilType_Depth24,
	RenderTextureDepthStencilType_Depth32,
	RenderTextureDepthStencilType_Depth32F,
	RenderTextureDepthStencilType_Stencil0,
	RenderTextureDepthStencilType_Stencil1,
	RenderTextureDepthStencilType_Stencil4,
	RenderTextureDepthStencilType_Stencil8,
	RenderTextureDepthStencilType_Stencil16,
	RenderTextureDepthStencilType_Depth_Stencil,
};

enum ConstantBufferType
{
	ConstantBufferType_None,
	ConstantBufferType_PerFrame,
	ConstantBufferType_PerCamera,
	ConstantBufferType_PerPass,
	//ShaderGlobalType_PerObject
};

enum RenderCommandType
{
	RenderCommandType_Draw
};