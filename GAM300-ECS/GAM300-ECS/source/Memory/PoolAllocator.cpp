/******************************************************************************/
/*!
\file       PoolAllocator.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       11 July 2022
\brief
  This file contains the implementations of PoolAllocator class. PoolAllocator
  divides memory (1 chunk) up into smaller chunks which then users can use.

  It can be used as a standalone memory allocator or be customised with
  FreeListAllocator.

  When provided an allocator (FreeListAllocator) in the constructor, PoolAllocator
  would take memory from the provided allocator. Else, it would just mallocs
  its own memory.
*/
/******************************************************************************/
#include "include/Memory/PoolAllocator.hpp"

#include "include/Memory/FreeListAllocator.hpp"
#include "include/Logging.hpp"

#include <iostream>
#include <stdint.h>
#include <stdlib.h>  // malloc, free
#include <algorithm> // max

#define UNUSED(expr) (void)expr

namespace Engine
{

PoolAllocator::PoolAllocator(const std::size_t totalSize_, const std::size_t chunkSize, Allocator* allocator) :
    Allocator(totalSize_), m_startPtr{ nullptr }, callocator{ nullptr }, isCustomAllocator{ false }
{
    //assert(chunkSize >= 8 && "Chunk size must be greater or equal to 8");
    LOG_ASSERT(chunkSize >= 8 && "Chunk size must be greater or equal to 8");
    //assert(totalSize_ % chunkSize == 0 && "Total Size must be a multiple of Chunk Size");
    LOG_ASSERT(totalSize_% chunkSize == 0 && "Total Size must be a multiple of Chunk Size");

    m_chunkSize = chunkSize;

    if (allocator == nullptr) 
    {
        this->Init();
    }
    else
    {
        callocator = allocator;
        isCustomAllocator = true;
        Init(totalSize_, allocator);
    }

}


PoolAllocator::~PoolAllocator()
{
    if (isCustomAllocator)
    {
        callocator->Free(m_startPtr);
    }
    else
    {
        free(m_startPtr);
    }
}


void PoolAllocator::Init()
{
    m_startPtr = malloc(m_totalSize);

    Reset();
}

void PoolAllocator::Init(const std::size_t totalSize_, Allocator* allocator)
{

    m_startPtr = allocator->Allocate(totalSize_, 8);

    Reset();
}


void *PoolAllocator::Allocate(const std::size_t size_, const std::size_t alignment_)
{
    UNUSED(size_);
    UNUSED(alignment_);

    return Allocate();
}

void* PoolAllocator::Allocate()
{
    Node* freeNode = m_freeList.pop();

    if (freeNode == nullptr)
    {
        return nullptr;
    }

    //assert(freeNode != nullptr && "The pool allocator is full");
    LOG_ASSERT(freeNode != nullptr && "The pool allocator is full");

    m_used += m_chunkSize;
    m_peak = (std::max)(m_peak, m_used);

#ifdef _DEBUG
    std::cout << "@Allocate"
        << "\t Start Pointer " << m_startPtr
        << "\t Free Node Address " << (void*)freeNode
        << "\t Memory Used " << m_used << std::endl;
#endif

    return (void*)freeNode;
}


void PoolAllocator::Free(void* ptr) 
{
    m_used -= m_chunkSize;

    m_freeList.push((Node*) ptr);

#ifdef _DEBUG
    std::cout << "@Free"
              << "\t Start Pointer " << m_startPtr 
              << "\t Freed Address " << ptr 
              << "\t Memory Used " << m_used << std::endl;
#endif
}


void PoolAllocator::Reset() 
{
    m_used = 0;
    m_peak = 0;

    // Create a linked-list with all free positions
    int nChunks = (int)(m_totalSize / m_chunkSize);

    for (int i = 0; i < nChunks; i++)
    {
        // Calculate address for each chunk and push to stack
        std::size_t address = (std::size_t)m_startPtr + i * m_chunkSize;

        m_freeList.push((Node*)address);
    }
}

} // end of namespace