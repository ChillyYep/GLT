#include "MultipleRenderTexture.h"
MultipleRenderTexture::MultipleRenderTexture(int width, int height, std::vector<AttachmentSetting> colorAttachmentSettings,
	AttachmentSetting depthAttachmentSetting, AttachmentSetting stencilAttachmentSetting)
{
	m_descriptor = RenderTargetDescriptor(width, height, colorAttachmentSettings, depthAttachmentSetting, stencilAttachmentSetting);
}