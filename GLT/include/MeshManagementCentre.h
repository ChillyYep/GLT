#pragma once
#include "Mesh.h"
#include "ManagementCentreBase.h"
/// <summary>
/// ��Щ�������Ⱦ��Mesh�ķ�����
/// </summary>
class MeshManagementCentre :public ManagementCentreBase<std::shared_ptr<Mesh>>
{
public:
	MeshManagementCentre() {}
	~MeshManagementCentre() {}

	void OnSubmit() override;
private:
	inline void DeallocateMesh(Mesh* mesh) { mesh->_Deallocate(); }
};
