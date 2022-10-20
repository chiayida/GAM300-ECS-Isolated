/******************************************************************************/
/*!
\file       SystemManager.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       19 August 2022
\brief
  This file contains the implementation of SystemManager classes.

  The SystemManager class is responsible of maintaining a record of registered
  systems and their signatures.

  Each system needs to have a signature set so that the manager can add the
  appropriate entities to each system's std::set of entities.

  std::set of entities should be updated whenever the entity's signature is updated.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "include/ECS/Architecture/SystemManager.hpp"

namespace Engine
{
	void SystemManager::DestroyEntity(Entity& e)
	{
		// Erase entity from all systems
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;

			system->mEntities.erase(e.GetEntityID());
		}
	}


	void SystemManager::DestroyEntity(EntityID& e)
	{
		// Erase entity from all systems
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;

			system->mEntities.erase(e);
		}
	}


	void SystemManager::EntitySignatureChanged(Entity& e, Signature signature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : mSystems)
		{
			auto const& name = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[name];

			// Entity's signature matches system signature
			if ((signature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(e.GetEntityID());
			}
			// Entity's signature does not match system signature
			else
			{
				system->mEntities.erase(e.GetEntityID());
			}
		}
	}


	void SystemManager::EntitySignatureChanged(EntityID& e, Signature signature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : mSystems)
		{
			auto const& name = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[name];

			// Entity's signature matches system signature
			if ((signature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(e);
			}
			// Entity's signature does not match system signature
			else
			{
				system->mEntities.erase(e);
			}
		}
	}

} // end of namespace