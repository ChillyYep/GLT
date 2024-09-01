#pragma once
#include <vector>
#include <Renderer.h>
struct CullingResult
{
	std::vector<Renderer*> m_renderers;
};