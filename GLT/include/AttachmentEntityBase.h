#pragma once
#include <RenderBuffer.h>
#include <Texture.h>

class AttachmentEntityBase
{
};

class RenderBufferAttachmentEntity :public AttachmentEntityBase
{
public:
	RenderBuffer* m_renderBufferPtr;
};

class TextureAttachmentEntity :public AttachmentEntityBase
{
public:
	Texture* m_texturePtr;
};