#pragma once
#include <ManagementCentreBase.h>
#include <Texture.h>
#include <memory>
/// <summary>
/// 那些会进入渲染的Texture的分配器
/// </summary>
class TextureManagementCentre :public ManagementCentreBase<std::shared_ptr<Texture>>
{
public:
	TextureManagementCentre() {}
	~TextureManagementCentre() {}

	void OnSubmit() override;
private:
	inline void UnloadTexture(Texture* texture) { texture->Unload(); }
};
