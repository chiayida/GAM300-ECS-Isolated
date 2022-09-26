/******************************************************************************/
/*!
\file       IComponent.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       18 August 2022
\brief
  This file contains the declaration and implementation of IComponent class.
  
  This class is to be inherited by other components.
*/
/******************************************************************************/
#pragma once

namespace Engine 
{
	class IComponent 
	{
	public:
		bool isActive = true;
	};
}