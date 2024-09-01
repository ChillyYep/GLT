#pragma once
enum class RenderType {
	Opaque,
	Transparent
};
struct FilterSettings
{
	RenderType m_renderType;
};