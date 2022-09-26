/******************************************************************************/
/*!
\file       PoolAllocator.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       11 July 2022
\brief
  This file contains the declaration of PoolAllocator class. PoolAllocator
  divides memory (1 chunk) up into smaller chunks which then users can use.

  It can be used as a standalone memory allocator or be customised with
  FreeListAllocator.

  When provided an allocator (FreeListAllocator) in the constructor, PoolAllocator
  would take memory from the provided allocator. Else, it would just mallocs
  its own memory.
*/
/******************************************************************************/
#include "Allocator.hpp"
#include "StackLinkedList.hpp"

namespace Engine
{

class PoolAllocator : public Allocator 
{
public:
    PoolAllocator(const std::size_t totalSize_, const std::size_t chunkSize, Allocator* allocator = nullptr);
    virtual ~PoolAllocator();

    virtual void Init() override;
    void Init(const std::size_t size, Allocator* allocator);

    virtual void* Allocate(const std::size_t size, const std::size_t alignment) override;
    void* Allocate();

    virtual void Free(void* ptr) override;
    virtual void Reset();

private:
    struct FreeHeader{};

    typedef StackLinkedList<FreeHeader>::Node Node;

    StackLinkedList<FreeHeader> m_freeList;

    std::size_t m_chunkSize;

    Allocator* callocator;
    bool isCustomAllocator;

    void* m_startPtr;
};

} // end of namespace