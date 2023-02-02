/******************************************************************************/
/*!
\file       TransformSystem.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       18 August 2022
\brief
  This file contains the implementation of TransformSystem class.

  It is just for me to play around and test whatever I am implementing at the
  moment.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#include "include/ECS/System/TransformSystem.hpp"
#include "include/ECS/Component/Transform.hpp"

#include <iostream>

namespace Engine
{
	void TransformSystem::Init()
	{
		for(auto entity : mEntities)
		{
			// do nothing
		}
	}

	void TransformSystem::Update()
	{
		std::cout << "entities: ";
		for (auto entity : mEntities)
		{
			// do nothing
			std::cout << entity << std::endl;
		}
	}

	void TransformSystem::Destroy()
	{
		
	}
}
