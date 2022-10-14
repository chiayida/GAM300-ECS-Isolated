/******************************************************************************/
/*!
\file       EntityManager.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       18 August 2022
\brief
  This file contains the function implementations of EntityManager class.

  The Entity Manager is in charge of distributing entity IDs and keeping record
  of which IDs are in use and which are not.
*/
/******************************************************************************/
#include "include/ECS/Architecture/EntityManager.hpp"
#include "include/Logging.hpp"

#include <string>

namespace Engine
{
	Entity::Entity(EntityID __id__, std::string __name__) : id{ __id__ }, is_child{ false }, name{ __name__ } {}

	Entity::Entity(EntityID __id__, EntityID __parent__, std::string __name__) :
		id{ __id__ }, parent{ __parent__ }, is_child{ true }, name{ __name__ } {};


	EntityID Entity::GetEntityID() const
	{
		return id;
	}


	void Entity::SetEntityName(std::string __name__)
	{
		name = __name__;
	}


	std::string Entity::GetEntityName() const
	{
		return name;
	}


	bool Entity::IsChild() const
	{
		return is_child;
	}


	EntityID Entity::GetParent() const
	{
		return parent;
	}


	void Entity::SetPrefab(std::string __prefab__)
	{
		prefab = __prefab__;
	}


	std::string Entity::GetPrefab() const
	{
		return prefab;
	}


	EntityManager::EntityManager() : EntityCount{ 0 }
	{
		// Initialize the queue that contains entity IDs up to MAX_ENTITIES.
		for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			AvailableEntities.emplace(entity);
		}
	}


	Entity EntityManager::CreateEntity(std::string __name__, EntityID _parent)
	{
		//assert(EntityCount < MAX_ENTITIES && "Number of entities exceeds MAX_ENTITIES");
		LOG_ASSERT(EntityCount < MAX_ENTITIES && "Number of entities exceeds MAX_ENTITIES");

		// Take an ID from the front of the queue
		EntityID id = AvailableEntities.front();
		AvailableEntities.pop();

		++EntityCount;

		if (_parent < MAX_ENTITIES)
		{
			return Entity(id, _parent, __name__);
		}

		if (__name__ == "")
		{
			__name__ = DEFAULT_ENTITY_NAME + std::to_string(id);
		}

		return Entity(id, __name__);
	}

	// Helper function to create a child object
	Entity EntityManager::CreateChild(EntityID parent, const std::string& __name__)
	{
		return CreateEntity(__name__, parent);
	}


	void EntityManager::DestroyEntity(Entity& e)
	{
		//assert(e < MAX_ENTITIES && "Entity is out of range.");
		LOG_ASSERT(e.GetEntityID() < MAX_ENTITIES && "Entity is out of range.");

		// Reset the entity's name
		e.SetEntityName(DEFAULT_ENTITY_NAME);

		// Resets the entity's signature bits (reset to zero)
		EntitiesSignatures[e.GetEntityID()].reset();

		// Move ID to the back of queue (to be reused)
		AvailableEntities.push(e.GetEntityID());

		--EntityCount;
	}


	void EntityManager::DestroyEntity(EntityID& e)
	{
		//assert(e < MAX_ENTITIES && "Entity is out of range.");
		LOG_ASSERT(e < MAX_ENTITIES && "Entity is out of range.");

		// Reset the entity's name
		//e.SetEntityName(DEFAULT_ENTITY_NAME);

		// Resets the entity's signature bits (reset to zero)
		EntitiesSignatures[e].reset();

		// Move ID to the back of queue (to be reused)
		AvailableEntities.push(e);

		--EntityCount;
	}


	void EntityManager::SetSignature(Entity& e, Signature s)
	{
		//assert(e < MAX_ENTITIES && "Entity is out of range.");
		LOG_ASSERT(e.GetEntityID() < MAX_ENTITIES && "Entity is out of range.");

		EntitiesSignatures[e.GetEntityID()] = s;
	}


	void EntityManager::SetSignature(EntityID& e, Signature s)
	{
		//assert(e < MAX_ENTITIES && "Entity is out of range.");
		LOG_ASSERT(e < MAX_ENTITIES && "Entity is out of range.");

		EntitiesSignatures[e] = s;
	}


	Signature EntityManager::GetSignature(Entity& e)
	{
		//assert(e < MAX_ENTITIES && "Entity is out of range.");
		LOG_ASSERT(e.GetEntityID() < MAX_ENTITIES && "Entity is out of range.");

		return EntitiesSignatures[e.GetEntityID()];
	}


	Signature EntityManager::GetSignature(EntityID& e)
	{
		LOG_ASSERT(e < MAX_ENTITIES && "Entity is out of range.");

		return EntitiesSignatures[e];
	}

} // end of namespace
RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace Engine;
	registration::class_<Entity>("AAEntity")
		.constructor<>()
		//.property_readonly("EntityID", &Entity::GetEntityID)
		//.property_readonly("EntityParent", &Entity::GetParent)
		//.property_readonly("IsChild", &Entity::IsChild)
		.property("name", &Entity::GetEntityName, &Entity::SetEntityName)
		.property("prefab", &Entity::GetPrefab, &Entity::SetPrefab)
		;
}