/******************************************************************************/
/*!
\file       FreeListAllocator.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       11 July 2022
\brief
  This file contains the implementation of FreeListAllocator class. FreeListAllocator
  allocates 1 huge chunk of memory which then is stored in the Free List for later use.
  Memory can be reused once the Free function is being called.

  It can be used as a standalone memory allocator or be customised with
  PoolListAllocator.
*/
/******************************************************************************/
#include "include/Memory/FreeListAllocator.hpp"
#include "include/Logging.hpp"

#include <iostream>
#include <stdlib.h>  // malloc, free
#include <limits>    // limits_max
#include <algorithm> // std::max

// std::size_t -> To get the memory address
namespace Engine
{

FreeListAllocator::FreeListAllocator(const std::size_t totalSize_, const Policy policy_) :
    Allocator(totalSize_), m_policy { policy_ }, m_startPtr { nullptr } 
{
    this->Init();
}


FreeListAllocator::~FreeListAllocator()
{
    free(m_startPtr);
    m_startPtr = nullptr;
}


void FreeListAllocator::FreeListAllocator::Init()
{
    // In case of memory, free memory first then allocate
    if (m_startPtr != nullptr)
    {
        free(m_startPtr);
        m_startPtr = nullptr;
    }
    m_startPtr = malloc(m_totalSize);

    std::cout << "FREELIST: " << m_startPtr << std::endl;

    Reset();
}


void* FreeListAllocator::Allocate(const std::size_t size_, const std::size_t alignment_)
{
    // Checks before continuing
    //assert("Allocation size must be bigger" && size_ >= sizeof(Node));
    LOG_ASSERT(size_ >= sizeof(Node) && "Allocation size must be bigger");
    //assert("Alignment must be at least 8" && alignment_ >= 8); // Aligned to AllocationHeader std::size_t
    LOG_ASSERT(alignment_ >= 8 && "Alignment must be at least 8");


    const std::size_t allocationHeaderSize = sizeof(FreeListAllocator::AllocationHeader);
    const std::size_t freeHeaderSize = sizeof(FreeListAllocator::FreeHeader);

    // Search through free list for a free block that has enough space to allocate data
    std::size_t padding;
    Node* allocatedNode;
    Node* prevNode;

    Find(size_, alignment_, padding, prevNode, allocatedNode);

    //assert(allocatedNode != nullptr && "Not enough memory"); // No space in free list for allocation
    LOG_ASSERT(allocatedNode != nullptr && "Not enough memory");


    const std::size_t alignmentPadding = padding - allocationHeaderSize;
    const std::size_t totalRequiredSize = size_ + padding;
    const std::size_t rest = allocatedNode->data.blockSize - totalRequiredSize;

    // We have to split the block into the data block and a free block of size 'rest'
    if (rest >= (sizeof(Node) + alignment_))
    {
        Node* newFreeNode = (Node*)((std::size_t)allocatedNode + totalRequiredSize);
        newFreeNode->data.blockSize = rest;
        m_freeList.insert(allocatedNode, newFreeNode);
    }
    m_freeList.remove(prevNode, allocatedNode);

    // Set up data block
    const std::size_t headerAddress = (std::size_t)allocatedNode + alignmentPadding;
    const std::size_t dataAddress = headerAddress + allocationHeaderSize;
    ((FreeListAllocator::AllocationHeader*)headerAddress)->blockSize = totalRequiredSize;
    ((FreeListAllocator::AllocationHeader*)headerAddress)->paddingSize = alignmentPadding;

    m_used += totalRequiredSize;
    m_peak = (std::max)(m_peak, m_used);

#ifdef _DEBUG
    std::cout << "@Allocate" 
              << "\t Header Address " << (void*)headerAddress 
              << "\t Data Address " << (void*)dataAddress 
              << "\t Total size of data (padding, header, data) " << ((FreeListAllocator::AllocationHeader*)headerAddress)->blockSize 
              << "\t Alignment Padding " << alignmentPadding 
              << "\t Padding " << padding 
              << "\t Memory Used " << m_used 
              << "\t Remaining Memory " << rest << std::endl;
#endif

    return (void*)dataAddress;
}


void FreeListAllocator::Free(void* ptr) 
{
    // Insert it in a sorted position by the address number
    const std::size_t currentAddress = (std::size_t)ptr;
    const std::size_t headerAddress = currentAddress - sizeof(FreeListAllocator::AllocationHeader);

    const FreeListAllocator::AllocationHeader* allocationHeader = (FreeListAllocator::AllocationHeader*)headerAddress;

    Node* freeNode = (Node*)headerAddress;
    freeNode->data.blockSize = allocationHeader->blockSize + allocationHeader->paddingSize;
    freeNode->nextNode = nullptr;

    Node* it = m_freeList.headNode;
    Node* itPrev = nullptr;
    while (it != nullptr) 
    {
        if (ptr < it) 
        {
            m_freeList.insert(itPrev, freeNode);
            break;
        }
        itPrev = it;
        it = it->nextNode;
    }

    m_used -= freeNode->data.blockSize;

    // Merge contiguous nodes
    MergeMemory(itPrev, freeNode);

#ifdef _DEBUG
    std::cout << "@Free" 
              << "\t Pointer Address " << ptr 
              << "\t Header Address " << (void*)freeNode 
              << "\t Size " << freeNode->data.blockSize 
              << "\t Memory Used " << m_used << std::endl;
#endif
}


void FreeListAllocator::Reset() 
{
    m_used = 0;
    m_peak = 0;

    Node* firstNode = (Node*)m_startPtr;
    firstNode->data.blockSize = m_totalSize;
    firstNode->nextNode = nullptr;

    m_freeList.headNode = nullptr;
    m_freeList.insert(nullptr, firstNode);
}


void FreeListAllocator::Find(const std::size_t size_, const std::size_t alignment_, std::size_t& padding,
    Node*& prevNode_, Node*& foundNode)
{
    switch (m_policy)
    {
    case Policy::FIND_FIRST:
    {
        FindFirst(size_, alignment_, padding, prevNode_, foundNode);
        break;
    }
    case Policy::FIND_BEST:
    {
        FindBest(size_, alignment_, padding, prevNode_, foundNode);
        break;
    }
    }
}


void FreeListAllocator::FindFirst(const std::size_t size_, const std::size_t alignment_, std::size_t& padding,
    Node*& prevNode_, Node*& foundNode)
{
    // Iterate list and return the first free block with a size >= than given size
    Node* it = m_freeList.headNode;
    Node* itPrev = nullptr;

    while (it != nullptr)
    {
        padding = CalculatePaddingWithHeader((std::size_t)it, alignment_, sizeof(FreeListAllocator::AllocationHeader));
        const std::size_t requiredSpace = size_ + padding;
        if (it->data.blockSize >= requiredSpace)
        {
            break;
        }
        itPrev = it;
        it = it->nextNode;
    }
    prevNode_ = itPrev;
    foundNode = it;
}


void FreeListAllocator::FindBest(const std::size_t size_, const std::size_t alignment_, std::size_t& padding,
    Node*& prevNode_, Node*& foundNode)
{
    // Iterate WHOLE list keeping a pointer to the best fit
    constexpr std::size_t smallestDiff = std::numeric_limits<std::size_t>::max();

    Node* bestBlock = nullptr;
    Node* it = m_freeList.headNode;
    Node* itPrev = nullptr;

    while (it != nullptr)
    {
        padding = CalculatePaddingWithHeader((std::size_t)it, alignment_, sizeof(FreeListAllocator::AllocationHeader));
        const std::size_t requiredSpace = size_ + padding;
        if (it->data.blockSize >= requiredSpace && (it->data.blockSize - requiredSpace < smallestDiff))
        {
            bestBlock = it;
        }
        itPrev = it;
        it = it->nextNode;
    }
    prevNode_ = itPrev;
    foundNode = bestBlock;
}


std::size_t FreeListAllocator::CalculatePadding(const std::size_t baseAddress, const std::size_t alignment_)
{
    const std::size_t multiplier = (baseAddress / alignment_) + 1; // + 1 as division rounds it down.
    const std::size_t alignedAddress = multiplier * alignment_;
    const std::size_t padding = alignedAddress - baseAddress;
    return padding;
}


std::size_t FreeListAllocator::CalculatePaddingWithHeader(const std::size_t baseAddress, 
    const std::size_t alignment_, const std::size_t headerSize)
{
    std::size_t padding = CalculatePadding(baseAddress, alignment_);
    std::size_t neededSpace = headerSize;

    if (padding < neededSpace) 
    {
        // Header does not fit - Calculate next aligned address that header fits
        neededSpace -= padding;

        // How many alignments I need to fit the header        
        if (neededSpace % alignment_ > 0)
        {
            padding += alignment_ * (1 + (neededSpace / alignment_));
        }
        else
        {
            padding += alignment_ * (neededSpace / alignment_);
        }
    }

    return padding;
}


void FreeListAllocator::MergeMemory(Node* prevNode, Node* freeNode)
{
    if (freeNode->nextNode != nullptr &&
        ((std::size_t)freeNode + freeNode->data.blockSize) == (std::size_t)freeNode->nextNode)
    {
        freeNode->data.blockSize += freeNode->nextNode->data.blockSize;
        m_freeList.remove(freeNode, freeNode->nextNode);

#ifdef _DEBUG
        std::cout << "\tMerging(n) " << (void*)freeNode 
            << " & " << (void*)freeNode->nextNode 
            << "\t Size " << freeNode->data.blockSize << std::endl;
#endif

    }

    if (prevNode != nullptr &&
        ((std::size_t)prevNode + prevNode->data.blockSize) == (std::size_t)freeNode)
    {
        prevNode->data.blockSize += freeNode->data.blockSize;
        m_freeList.remove(prevNode, freeNode);

#ifdef _DEBUG
        std::cout << "\tMerging(p) " << (void*)prevNode
                  << " & " << (void*)freeNode 
                  << "\t Size " << prevNode->data.blockSize << std::endl;
#endif

    }
}

} // end of namespace






// Deprecated code, might reuse in future for improvements
/*
// If no space in free list for allocation (out of memory), add another "page" of free list.
void FreeListAllocator::NewPage()
{
    std::cout << "new page added" << std::endl;

    void* newMemory = malloc(m_totalSize);
    const std::size_t new_MemoryAddress = (std::size_t)newMemory;

    Node* freeNode = (Node*)new_MemoryAddress;
    freeNode->data.blockSize = m_totalSize;
    freeNode->nextNode = nullptr;

    Node* it = m_freeList.headNode;
    Node* itPrev = nullptr;

    while (it != nullptr)
    {
        itPrev = it;
        it = it->nextNode;
    }
    m_totalSize += m_totalSize;

    // Merge contiguous nodes
    MergeMemory(itPrev, freeNode);

    Find(size_, alignment_, padding, prevNode, allocatedNode);
}
*/