/******************************************************************************/
/*!
\file       FreeListAllocator.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       11 July 2022
\brief
  This file contains the declaration of FreeListAllocator class. FreeListAllocator
  allocates 1 huge chunk of memory which then is stored in the Free List for later use.
  Memory can be reused once the Free function is being called.

  It can be used as a standalone memory allocator or be customised with
  PoolListAllocator.
*/
/******************************************************************************/
#pragma once

#include "Allocator.hpp"
#include "SinglyLinkedList.hpp"

namespace Engine
{

class FreeListAllocator : public Allocator 
{
public:
    enum class Policy
    {
        FIND_FIRST, // FindFirst function, first memory block that fits size. 
        FIND_BEST   // FindBest function, smallest memory block that fits size
    };

    FreeListAllocator(const std::size_t totalSize_, const Policy policy_ = FreeListAllocator::Policy::FIND_FIRST);
    virtual ~FreeListAllocator();

    virtual void Init() override;
    virtual void* Allocate(const std::size_t size_, const std::size_t alignment_ = 0) override;
    virtual void Free(void* ptr) override;

    virtual void Reset() override;

private:
    struct FreeHeader
    {
        std::size_t blockSize;
    };

    struct AllocationHeader
    {
        std::size_t blockSize;
        std::size_t paddingSize;
    };

    typedef SinglyLinkedList<FreeHeader>::Node Node;

    void* m_startPtr; // Starting position of memory
    Policy m_policy;  // Determines allocation type
    SinglyLinkedList<FreeHeader> m_freeList;

    void Find(const std::size_t size_, const std::size_t alignment_, std::size_t& padding, Node*& prevNode_, Node*& foundNode);
    void FindFirst(const std::size_t size_, const std::size_t alignment_, std::size_t& padding, Node*& prevNode_, Node*& foundNode);
    void FindBest(const std::size_t size_, const std::size_t alignment_, std::size_t& padding, Node*& prevNode_, Node*& foundNode);

    std::size_t CalculatePadding(const std::size_t baseAddress, const std::size_t alignment_);
    std::size_t CalculatePaddingWithHeader(const std::size_t baseAddress, const std::size_t alignment_, const std::size_t headerSize);

    void MergeMemory(Node* prevBlock, Node* freeBlock);
};

} // end of namespace