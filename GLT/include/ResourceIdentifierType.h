#pragma once
enum class ResourceIdentifierType {
	Invalid,
	Mesh,
	Texture,
	Sampler,
	RenderTarget
};
template<typename T>
struct resourceidentifier_traits
{
	ResourceIdentifierType value = ResourceIdentifierType::Invalid;
};
