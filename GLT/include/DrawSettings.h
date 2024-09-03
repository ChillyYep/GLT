#pragma once
#include <GLMath.h>

enum class SortType {
	Near2Far,
	Far2Near,
	Custom
};
struct DrawSettings
{
	glm::vec3 m_cameraPos;
	SortType m_sortType;
};