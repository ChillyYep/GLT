#pragma once
#include "Mesh.h"
#include "ManagementCentreBase.h"
/// <summary>
/// 那些会进入渲染的Mesh的分配器
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
