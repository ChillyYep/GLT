#pragma once
#include <ManagementCentreBase.h>
#include <Texture.h>
/// <summary>
/// 那些会进入渲染的Texture的分配器
/// </summary>
class TextureManagementCentre :public ManagementCentreBase<Texture*>
{
public:
	TextureManagementCentre() {}
	~TextureManagementCentre() {}

	void onSubmit() override;
private:
	inline void unloadTexture(Texture* texture) { texture->unload(); }
};
