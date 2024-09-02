#pragma once
enum class RenderType {
	Opaque = 1,
	Transparent = 1 << 1
};
struct FilterSettings
{
	RenderType m_renderType;
};