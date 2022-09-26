/******************************************************************************/
/*!
\file       Allocator.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       11 July 2022
\brief
  This file contains the declaration of the Allocator class. This class is to
  be inherited by both FreeListAllocator and PoolListAllocator.
*/
/******************************************************************************/
#pragma once

#include <cstddef> // std::size_t

namespace Engine
{
class Allocator 
{
public:
	Allocator(std::size_t totalSize_);
	virtual ~Allocator() { m_totalSize = 0; };

	// Pure virtual functions, to be implemented by inherited classes (Pool, Free list allocator)
	virtual void Init() = 0;
	virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) = 0;
	virtual void Free(void* ptr) = 0;
	virtual void Reset() = 0;

protected:
	// Accessible to inherited classes
	std::size_t m_totalSize;
	std::size_t m_used;
	std::size_t m_peak;
};

} // end of namespace