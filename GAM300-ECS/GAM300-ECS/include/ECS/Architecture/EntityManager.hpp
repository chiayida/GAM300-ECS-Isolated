/******************************************************************************/
/*!
\file       EntityManager.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       18 August 2022
\brief
  This file contains the declaration of EntityManager class.
  
  The Entity Manager is in charge of distributing entity IDs and keeping record 
  of which IDs are in use and which are not.

  A queue is used where it will contain every valid entity ID up to MAX_ENTITIES.
  When an entity is created, it takes the ID at the front of the queue. 
  When an entity is destroyed, it puts the ID at the back of the queue.
*/
/******************************************************************************/
#pragma once

#include "ECS.hpp"

#include <queue>
#include <array>

#include <lib/rttr/type>
#include <lib/rttr/registration.h>

namespace Engine
{
	#define DEFAULT_ENTITY_NAME "Entity "

	class Entity
	{
	public:
		Entity(EntityID __id__ = MAX_ENTITIES + 1, std::string __name__ = DEFAULT_ENTITY_NAME);
		Entity(EntityID __id__, EntityID __parent__, std::string __name__ = DEFAULT_ENTITY_NAME);

		// Getter, Setter
		EntityID GetEntityID() const;

		void SetEntityName(std::string __name__);
		std::string GetEntityName() const;

		bool IsChild() const;
		EntityID GetParent() const;

		void SetPrefab(std::string __prefab__);
		std::string GetPrefab() const;

		RTTR_ENABLE()

	private:
		EntityID id;

		EntityID parent; // parent id given that is_child is true
		bool is_child;

		std::string name;
		std::string prefab{};
	};


	class EntityManager
	{
	public:
		EntityManager();

		Entity CreateEntity(std::string __name__ = "", EntityID _parent = MAX_ENTITIES + 1);
		Entity CreateChild(EntityID _parent, const std::string& __name__ = "");

		void DestroyEntity(Entity& e);
		void DestroyEntity(EntityID& e);

		void SetSignature(Entity& e, Signature s);
		void SetSignature(EntityID& e, Signature s);

		Signature GetSignature(Entity& e);
		Signature GetSignature(EntityID& e);

	private:
		// Container of unused entity IDs
		std::queue<EntityID> AvailableEntities{};

		// Container of signatures where index corresponds to entity ID
		std::array<Signature, MAX_ENTITIES> EntitiesSignatures{};

		// Total number of active entities - used to keep limits on how many exist
		unsigned int EntityCount;
	};

} // end of namespace