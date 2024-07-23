#pragma once
#include <Enums.h>
#include <vector>
#include <Allocator.h>

struct RenderCommand
{
	RenderCommandType m_commandType;
	void* offset;
};
class CommandBuffer
{
public:
	CommandBuffer(Allocator* allocator)
	{
		m_allocator = allocator;
	}
	~CommandBuffer() {}
private:
	std::vector<RenderCommand> m_cmdList;
	Allocator* m_allocator;
};
