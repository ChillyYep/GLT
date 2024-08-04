#pragma once
#include <ManagementCentreBase.h>
#include <Texture.h>
#include <memory>
/// <summary>
/// ��Щ�������Ⱦ��Texture�ķ�����
/// </summary>
class TextureManagementCentre :public ManagementCentreBase<std::shared_ptr<Texture>>
{
public:
	TextureManagementCentre() {}
	~TextureManagementCentre() {}

	void onSubmit() override;
private:
	inline void unloadTexture(Texture* texture) { texture->unload(); }
};
