#pragma once
#include "Mesh.h"
#include "ManagementCentreBase.h"
/// <summary>
/// ��Щ�������Ⱦ��Mesh�ķ�����
/// </summary>
class MeshManagementCentre :public ManagementCentreBase<SubMesh*>
{
public:
	MeshManagementCentre() {}
	~MeshManagementCentre() {}

	void onSubmit() override;
private:
	inline void deallocateMesh(SubMesh* mesh) { mesh->_deallocate(); }
};
