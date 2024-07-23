#pragma once
#include <iostream>
#include <cassert>
#include <cstdint>
#ifndef ALIGN
#define ALIGN(x, a)         (((x) + ((a) - 1)) & ~((a) - 1))
#endif

struct BlockHeader
{
	BlockHeader* pNext;
};
struct PageHeader
{
	PageHeader* pNext;

	BlockHeader* FirstBlock() {
		return reinterpret_cast<BlockHeader*>(this + 1);
	}
};
class Allocator
{
public:
	Allocator(uint32_t pageSize, uint32_t dataSize, uint32_t alignmentSize);
	~Allocator();
	void Reset(uint32_t pageSize, uint32_t dataSize, uint32_t alignment);
	void* Allocate();
	BlockHeader* NextBlock(BlockHeader* pBlock);
	void Free(void* p);
	void FreeAll();
private:
#ifdef _DEBUG
	void FillFreePage(PageHeader* pPage);

	void FillFreeBlock(BlockHeader* pBlock);

	void FillAllocatedBlock(BlockHeader* pBlock);
#endif


	PageHeader* m_pPageList;
	BlockHeader* m_pNextFreeBlock;

	uint32_t m_szDataSize;
	uint32_t m_szAlignmentSize;
	uint32_t m_szPageSize;
	uint32_t m_szBlockSize;
	uint32_t m_nBlocksPerPage;

	uint32_t m_nBlocks;
	uint32_t m_nPages;
	uint32_t m_nFreeBlocks;

};
