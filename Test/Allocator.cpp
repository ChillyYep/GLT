#include "Allocator.h"
#ifdef _DEBUG
static const uint8_t PATTERN_ALIGN = 0xFC;
static const uint8_t PATTERN_ALLOC = 0xFD;
static const uint8_t PATTERN_FREE = 0xFE;
#endif // _DEBUG
Allocator::Allocator(size_t pageSize, size_t dataSize, size_t alignmentSize)
{
	Reset(pageSize, dataSize, alignmentSize);
}
Allocator::~Allocator()
{
	FreeAll();
}

void Allocator::Reset(size_t pageSize, size_t dataSize, size_t alignment)
{
	FreeAll();

	m_szDataSize = dataSize;
	m_szPageSize = pageSize;

	size_t minimal_size = (sizeof(BlockHeader) > m_szDataSize) ? sizeof(BlockHeader) : m_szDataSize;

	assert(alignment > 0 && ((alignment & (alignment - 1)) == 0));

	m_szBlockSize = ALIGN(minimal_size, alignment);

	m_szAlignmentSize = m_szBlockSize - minimal_size;

	// 每一页起始4字节存放的是下一页的指针
	m_nBlocksPerPage = (m_szPageSize - sizeof(PageHeader)) / m_szBlockSize;
}
void* Allocator::Allocate()
{
	if (!m_pNextFreeBlock)
	{
		PageHeader* pNewPage = reinterpret_cast<PageHeader*>(new uint8_t[m_szPageSize]);
		++m_nPages;
		m_nBlocks += m_nBlocksPerPage;
		m_nFreeBlocks += m_nBlocksPerPage;
#ifdef _DEBUG
		FillFreePage(pNewPage);
#endif // _DEBUG

		if (m_pPageList != nullptr)
		{
			pNewPage->pNext = m_pPageList;
		}
		m_pPageList = pNewPage;
		// 每一个block的开头4字节都用于存储下一个block的地址
		BlockHeader* pBlock = pNewPage->FirstBlock();
		for (uint32_t i = 0;i < m_nBlocksPerPage - 1;++i)
		{
			auto nextBlock = NextBlock(pBlock);
			pBlock->pNext = nextBlock;
			pBlock = nextBlock;
		}
		// 最后一个Block的Next指针为null
		pBlock->pNext = nullptr;

		m_pNextFreeBlock = pNewPage->FirstBlock();
	}
	// m_pNextFreeBlock为首个空闲未使用的Block
	BlockHeader* readyUseBlock = m_pNextFreeBlock;
	m_pNextFreeBlock = m_pNextFreeBlock->pNext;
	--m_nFreeBlocks;
#ifdef _DEBUG
	FillAllocatedBlock(readyUseBlock);
#endif // _DEBUG

	return reinterpret_cast<void*>(readyUseBlock);
}

BlockHeader* Allocator::NextBlock(BlockHeader* pBlock)
{
	return reinterpret_cast<BlockHeader*>(reinterpret_cast<uint8_t*>(pBlock) + m_szBlockSize);
}

void Allocator::Free(void* p) {
	BlockHeader* block = reinterpret_cast<BlockHeader*>(p);
#ifdef _DEBUG
	FillFreeBlock(block);
#endif // _DEBUG

	block->pNext = m_pNextFreeBlock;
	m_pNextFreeBlock = block;
	++m_nFreeBlocks;
}

void Allocator::FreeAll()
{
	auto curPage = m_pPageList;
	while (curPage != nullptr)
	{
		auto watiReleasePage = curPage;
		curPage = curPage->pNext;
		delete[] reinterpret_cast<uint8_t*>(watiReleasePage);
	}
	m_pPageList = nullptr;
	m_pNextFreeBlock = nullptr;

	m_nPages = 0;
	m_nBlocks = 0;
	m_nFreeBlocks = 0;
}


#ifdef _DEBUG
void Allocator::FillFreePage(PageHeader* pPage)
{
	pPage->pNext = nullptr;

	BlockHeader* pBlock = pPage->FirstBlock();
	for (uint32_t i = 0;i < m_nBlocksPerPage;++i)
	{
		FillFreeBlock(pBlock);
		pBlock = NextBlock(pBlock);
	}
}

void Allocator::FillFreeBlock(BlockHeader* pBlock)
{
	// Block的next指针以及部分非对齐的内存
	memset(pBlock, PATTERN_FREE, m_szBlockSize - m_szAlignmentSize);

	// 对齐的内存
	memset(reinterpret_cast<uint8_t*>(pBlock) + m_szBlockSize - m_szAlignmentSize, PATTERN_ALIGN, m_szAlignmentSize);
}

void Allocator::FillAllocatedBlock(BlockHeader* pBlock)
{
	// Block的next指针以及部分非对齐的内存
	memset(pBlock, PATTERN_FREE, m_szBlockSize - m_szAlignmentSize);

	// 对齐的内存
	memset(reinterpret_cast<uint8_t*>(pBlock) + m_szBlockSize - m_szAlignmentSize, PATTERN_ALLOC, m_szAlignmentSize);
}
#endif