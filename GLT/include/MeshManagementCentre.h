#pragma once
#include "Mesh.h"
#include "ManagementCentreBase.h"
#include <memory>
/// <summary>
/// ��Щ�������Ⱦ��Mesh�ķ�����
/// </summary>
class MeshManagementCentre :public ManagementCentreBase<std::shared_ptr<Mesh>>
{
public:
	MeshManagementCentre() {}
	~MeshManagementCentre() {}

	void onSubmit() override;
private:
	inline void deallocateMesh(Mesh* mesh) { mesh->_deallocate(); }
};
