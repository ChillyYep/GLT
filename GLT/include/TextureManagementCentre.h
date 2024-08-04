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

	void OnSubmit() override;
private:
	inline void UnloadTexture(Texture* texture) { texture->Unload(); }
};
