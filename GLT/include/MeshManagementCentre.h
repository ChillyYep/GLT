#pragma once
#include "Mesh.h"
#include "ManagementCentreBase.h"
/// <summary>
/// ��Щ�������Ⱦ��Mesh�ķ�����
/// </summary>
class MeshManagementCentre :public ManagementCentreBase<Mesh*>
{
public:
	MeshManagementCentre() {}
	~MeshManagementCentre() {}

	void onSubmit() override;
private:
	inline void deallocateMesh(Mesh* mesh) { mesh->_deallocate(); }
};
