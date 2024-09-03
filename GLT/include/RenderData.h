#pragma once
#include <GLMath.h>
#include <vector>

struct CameraData {
	glm::vec3 m_worldPos;
};
struct RenderData
{
	int m_curCameraIndex;
	std::vector<CameraData> m_cameraDatas;
};