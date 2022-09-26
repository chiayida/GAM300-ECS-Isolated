/******************************************************************************/
/*!
\file       Allocator.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       11 July 2022
\brief
  This file contains the implementation of the Allocator class. This class is to
  be inherited by both FreeListAllocator and PoolListAllocator.
*/
/******************************************************************************/
#include "include/Memory/Allocator.hpp"

namespace Engine 
{
// Default Constructor for class Allocator
Allocator::Allocator(std::size_t totalSize_) : 
	m_totalSize{ totalSize_ }, m_used{ 0 }, m_peak{ 0 } {}

} // end of namespace