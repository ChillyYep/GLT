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

	void updateModifiedObjects() override
	{
		for (const auto& resource : m_allObjects)
		{
			auto texture = resource.second.m_target;
			if (texture != nullptr && texture->getDirty())
			{
				m_modifiedObjects.insert(texture->getInstanceId());
				texture->setDirty(false);
			}
		}
	}

	void onSubmit() override;
private:
	inline void unloadTexture(Texture* texture) { texture->unload(); }

};
