/******************************************************************************/
/*!
\file       CustomAllocator.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       11 July 2022
\brief
  This file contains the declaration and implementation of Templated
  CustomAllocator class. CustomAllocator is a combination of both PoolAllocator
  and FreeListAllocator where FreeListAllocator would mallocs a huge chunk of
  memory at the start. PoolAllocator would then divides allocated memory into
  smaller chunks for each individual component array.

  CustomAllocator would also explicity calls the component's constructor and
  destructor when allocating and freeing memory, making it more seemless for the
  user.

  A MINIMUM SIZE of 16 bytes for the Component is required, else CustomAllocator
  would fail.
*/
/******************************************************************************/
#include "PoolAllocator.hpp"
#include "StackLinkedList.hpp"

#include <iostream>

namespace Engine
{

// T - Component Type
// N - Number of elements per chunk.
// Templated Pool Allocator
template <typename T, unsigned N>
class CustomAllocator
{
public:
    CustomAllocator(Allocator* allocator_);

    template <typename... argv>
    T* Allocate(argv... args);

    void Free(T* ptr);

    CustomAllocator() = delete;
    CustomAllocator(const CustomAllocator&) = delete;
    CustomAllocator& operator=(const CustomAllocator&) = delete;

private:
    PoolAllocator m_allocator;
};



// Templated CustomAllocator Class function definitions
// MINIMUM 16 BYTES 


template <typename T, unsigned N>
CustomAllocator<T, N>::CustomAllocator(Allocator* allocator_) : 
    m_allocator((sizeof(T) * N), sizeof(T), allocator_) {}


// Variadic function for all Component type constructor
template <typename T, unsigned N>
template <typename... argv>
T* CustomAllocator<T, N>::Allocate(argv... args)
{
    void* ptr = m_allocator.Allocate();

    // Explicitly calls constructor with Placement New Operator
    new (ptr) T(args ...);

    return reinterpret_cast<T*>(ptr);
}


template <typename T, unsigned N>
void CustomAllocator<T, N>::Free(T* ptr)
{
    // Explicitly calls destructor for component
    ptr->~T();

    m_allocator.Free(ptr);
}


} // end of namespace