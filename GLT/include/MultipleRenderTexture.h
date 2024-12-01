#pragma once
#include <Texture.h>
#include <RenderTargetTextureBase.h>

class MultipleRenderTexture :public RenderTargetTextureBase
{
public:
	MultipleRenderTexture(int width, int height, std::vector<AttachmentSetting> colorAttachmentSettings,
		AttachmentSetting depthAttachmentSetting, AttachmentSetting stencilAttachmentSetting);
	~MultipleRenderTexture() {}

	void setColorAttachmentSampleEnabled(int colorAttachmentIndex, bool enabled)
	{
		assert(!m_isCreated && "Please call function \"setColorAttachmentSampleEnabled\" before \"create\"!");
		if (colorAttachmentIndex < m_descriptor.m_colorAttachmentSettings.size() && colorAttachmentIndex >= 0)
		{
			m_descriptor.m_colorAttachmentSettings[colorAttachmentIndex].m_enableSample = enabled;
		}
	}
private:
};