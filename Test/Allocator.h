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
	Allocator(size_t pageSize, size_t dataSize, size_t alignmentSize);
	~Allocator();
	void Reset(size_t pageSize, size_t dataSize, size_t alignment);
	void* Allocate();
	BlockHeader* NextBlock(BlockHeader* pBlock);
	void Free(void* p);
	void FreeAll();
#ifdef _DEBUG
	inline void LogMemInfo()
	{
		auto curPage = m_pPageList;
		while (curPage != nullptr)
		{
			auto page = curPage;
			curPage = curPage->pNext;

			auto pBlock = page->FirstBlock();
			for (uint32_t i = 0;i < m_nBlocksPerPage;++i)
			{
				auto innerBlockPtr = reinterpret_cast<uint8_t*>(pBlock);

				std::cout << "Block" << i << std::endl;
				for (uint32_t j = 0;j < m_szBlockSize;++j)
				{
					std::cout /*<< j << ":"*/ << (int)(*(innerBlockPtr + j)) << "\t";
					if ((j + 1) % 8 == 0)
					{
						std::cout << std::endl;
					}
				}
				pBlock = NextBlock(pBlock);
			}
			std::cout << std::endl;
		}
	}
#endif // _DEBUG

private:
#ifdef _DEBUG
	void FillFreePage(PageHeader* pPage);

	void FillFreeBlock(BlockHeader* pBlock);

	void FillAllocatedBlock(BlockHeader* pBlock);
#endif


	PageHeader* m_pPageList;
	BlockHeader* m_pNextFreeBlock;

	size_t m_szDataSize;
	size_t m_szAlignmentSize;
	size_t m_szPageSize;
	size_t m_szBlockSize;
	uint32_t m_nBlocksPerPage;

	uint32_t m_nBlocks;
	uint32_t m_nPages;
	uint32_t m_nFreeBlocks;

};
