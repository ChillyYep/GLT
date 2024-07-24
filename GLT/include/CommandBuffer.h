#pragma once
#include <Enums.h>
#include <vector>
#include <Allocator.h>

struct RenderCommand
{
	RenderCommandType m_commandType;
	int m_offset;
	int m_size;
};
class CommandBuffer
{
public:
	CommandBuffer(Allocator* allocator)
	{
		m_allocator = allocator;
	}
	~CommandBuffer() {}

	void Release()
	{

	}
private:
	std::vector<RenderCommand> m_cmdList;
	Allocator* m_allocator;
};
