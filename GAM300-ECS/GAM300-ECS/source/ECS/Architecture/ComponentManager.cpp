/******************************************************************************/
/*!
\file       ComponentManager.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       19 August 2022
\brief
  This file contains the implementation of ComponentManager class functions.

  ComponentManager is in charge of handling the different ComponentArrays when
  a component has to be added or removed.

  It contains a ComponentType variable that increments by one whenever a component
  is registered.

  It also have a unique key which is a map to ComponentArray to the ComponentType.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "include/ECS/Architecture/ComponentManager.hpp"

namespace Engine
{
	void ComponentManager::DestroyEntity(Entity& e)
	{
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(e);
		}
	}


	void ComponentManager::DestroyEntity(EntityID& e)
	{
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(e);
		}
	}


	void ComponentManager::FreeCustomAllocator()
	{
		for (auto const& pair : mComponentArrays)
		{
			pair.second->FreeComponentArray();
		}
	}

} // end of namespace