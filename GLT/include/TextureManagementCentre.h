#pragma once
#include <ManagementCentreBase.h>
#include <Texture.h>
/// <summary>
/// ��Щ�������Ⱦ��Texture�ķ�����
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
