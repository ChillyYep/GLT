#pragma once
#include <Object.h>
#include <RenderTarget.h>
#include <LogicResourceManager.h>

class RenderTargetTextureBase :public Object
{
public:
	RenderTargetTextureBase() :m_renderTarget(nullptr), m_isCreated(false) {}
	unsigned int getRTInstanceId() { return m_renderTarget != nullptr ? m_renderTarget->getInstanceId() : 0; }

	virtual void create();
	virtual void release();

	void setDepthAttachmentSampleEnabled(bool enabled)
	{
		assert(!m_isCreated && "Please call function \"setColorAttachmentSampleEnabled\" before \"create\"!");
		m_descriptor.m_depthAttachmentSetting.m_enableSample = enabled;
	}
	void setStencilAttachmentSampleEnabled(bool enabled)
	{
		assert(!m_isCreated && "Please call function \"setColorAttachmentSampleEnabled\" before \"create\"!");
		m_descriptor.m_stencilAttachmentSetting.m_enableSample = enabled;
	}

	__GET_SET_PROPERTY__(Descriptor, RenderTargetDescriptor, m_descriptor)
protected:

	void release(AttachmentEntityWrapper& attachment);

	bool m_isCreated;

	RenderTarget* m_renderTarget;

	RenderTargetDescriptor m_descriptor;
};