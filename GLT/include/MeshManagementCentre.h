#pragma once
#include "Mesh.h"
#include "ManagementCentreBase.h"
/// <summary>
/// 那些会进入渲染的Mesh的分配器
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
