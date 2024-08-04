#include "Texture.h"

GLenum Texture::textureType2TextureTarget(TextureType textureType)
{
	switch (textureType)
	{
	case TextureType::Texture1D:
		return GL_TEXTURE_1D;
	case TextureType::Texture1DArray:
		return GL_TEXTURE_1D_ARRAY;
	case TextureType::Texture2D:
		return GL_TEXTURE_2D;
	case TextureType::Texture2DArray:
		return GL_TEXTURE_2D_ARRAY;
	case TextureType::Texture2DMultiSample:
		return GL_TEXTURE_2D_MULTISAMPLE;
	case TextureType::Texture2DMultiSampleArray:
		return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	case TextureType::Texture3D:
		return GL_TEXTURE_3D;
	case TextureType::CubeMap:
		return GL_TEXTURE_CUBE_MAP;
	case TextureType::CubeMapArray:
		return GL_TEXTURE_CUBE_MAP_ARRAY;
	case TextureType::Buffer:
		return GL_TEXTURE_BUFFER;
	default:
		break;
	}
	return GL_NONE;
}