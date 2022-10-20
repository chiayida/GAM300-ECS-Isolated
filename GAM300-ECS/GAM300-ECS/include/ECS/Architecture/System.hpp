/******************************************************************************/
/*!
\file       System.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       19 August 2022
\brief
  This file contains the declaration and implementation of System class.

  The System class has only a std::set of entities. 
  Calls insert() and erase() to add and remove entities.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "ECS.hpp"
#include <set>

#define UNUSED(expr) (void)expr

namespace Engine
{
	class System
	{
	public:
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Update(float dt) { UNUSED(dt); }
		virtual void Destroy() = 0;

		std::set<EntityID> mEntities{};
	};

} // end of namespace