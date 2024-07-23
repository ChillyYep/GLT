#include "MeshManagementCentre.h"

void MeshManagementCentre::OnSubmit()
{
	for (const auto& meshPair : m_allObjects)
	{
		auto& meshItem = meshPair.second;
		auto& mesh = meshItem.m_target;
		if (!mesh->GetCanReadWrite())
		{
			DeallocateMesh(mesh.get());
		}
	}
}
