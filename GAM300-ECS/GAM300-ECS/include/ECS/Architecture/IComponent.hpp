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

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <lib/rttr/type>
#include <lib/rttr/registration.h>

namespace Engine 
{
	class alignas(8) IComponent 
	{
	public:
		bool isActive = true;

		RTTR_ENABLE();
	};
}