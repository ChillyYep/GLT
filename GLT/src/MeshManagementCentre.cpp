#include "MeshManagementCentre.h"

void MeshManagementCentre::onSubmit()
{
	for (const auto& meshPair : m_allObjects)
	{
		auto& meshItem = meshPair.second;
		auto& mesh = meshItem.m_target;
		if (!mesh->getCanReadWrite())
		{
			deallocateMesh(mesh.get());
		}
	}
}
