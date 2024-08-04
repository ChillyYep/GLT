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
enum class TextureFormat {

};