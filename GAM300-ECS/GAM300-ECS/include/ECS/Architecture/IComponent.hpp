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

#include <lib/rttr/type>
#include <lib/rttr/registration.h>

namespace Engine 
{
	class IComponent 
	{
	public:
		bool isActive = true;
		unsigned int padding{};

		RTTR_ENABLE();
	};
}