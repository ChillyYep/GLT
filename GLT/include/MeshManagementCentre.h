#pragma once
#include "Mesh.h"
#include "ManagementCentreBase.h"
/// <summary>
/// 那些会进入渲染的Mesh的分配器
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
