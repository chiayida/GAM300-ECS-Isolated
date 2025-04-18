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

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#include "include/ECS/Architecture/EntityManager.hpp"
#include "include/Logging.hpp"

#include <string>

namespace Engine
{
	Entity::Entity(EntityID __id__, std::string __name__) : id{ __id__ }, parent{ MAX_ENTITIES + 1 }, is_parent{ false }, is_child{ false }, isActive{ true }, name{ __name__ } {}

	Entity::Entity(EntityID __id__, EntityID __parent__, std::string __name__) :
		id{ __id__ }, parent{ __parent__ }, name{ __name__ }, is_parent{ false }, is_child{ true }, isActive{ true } {};


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


	void Entity::Copy(const Entity& rhs)
	{
		is_child = rhs.is_child;
		is_parent = rhs.is_parent;
		isActive = rhs.isActive;

		prefab = rhs.GetPrefab();
	}


	bool Entity::IsChild() const
	{
		return is_child;
	}


	bool Entity::isParent() const
	{
		return is_parent;
	}


	void Entity::SetIs_Child(bool val)
	{
		is_child = val;
	}


	void Entity::SetParentID(EntityID ids)
	{
		parent = ids;
	}


	void Entity::SetIs_Parent(bool val)
	{
		is_parent = val;
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


	void Entity::SetKeyTag(std::string __tag__)
	{
		tag = __tag__;
	}


	std::string Entity::GetKeyTag() const
	{
		return tag;
	}
		

	void Entity::SetIs_Active(bool val)
	{
		isActive = val;
	}


	bool Entity::GetIsActive()
	{
		return isActive;
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
		LOG_ASSERT(EntityCount < MAX_ENTITIES && "Number of entities exceeds MAX_ENTITIES");

		// Take an ID from the front of the queue
		EntityID id = AvailableEntities.front();
		AvailableEntities.pop();

		++EntityCount;

		if (__name__ == "")
		{
			__name__ = DEFAULT_ENTITY_NAME + std::to_string(id);
		}

		if (_parent < MAX_ENTITIES)
		{
			return Entity(id, _parent, __name__);
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
		LOG_ASSERT(e.GetEntityID() < MAX_ENTITIES && "Entity is out of range.");

		// Resets the signature bits (reset to zero)
		EntitiesSignatures[e.GetEntityID()].reset();
		EntitiesTags[e.GetEntityID()].reset();

		// Move ID to the back of queue (to be reused)
		AvailableEntities.push(e.GetEntityID());

		--EntityCount;
	}


	void EntityManager::DestroyEntity(EntityID& e)
	{
		LOG_ASSERT(e < MAX_ENTITIES && "Entity is out of range.");

		// Resets the signature bits (reset to zero)
		EntitiesSignatures[e].reset();
		EntitiesTags[e].reset();

		// Move ID to the back of queue (to be reused)
		AvailableEntities.push(e);

		--EntityCount;
	}


	void EntityManager::SetSignature(Entity& e, Signature s)
	{
		LOG_ASSERT(e.GetEntityID() < MAX_ENTITIES && "Entity is out of range.");

		EntitiesSignatures[e.GetEntityID()] = s;
	}


	void EntityManager::SetSignature(EntityID& e, Signature s)
	{
		LOG_ASSERT(e < MAX_ENTITIES && "Entity is out of range.");

		EntitiesSignatures[e] = s;
	}


	void EntityManager::SetTag(EntityID e, Tag t)
	{
		LOG_ASSERT(e < MAX_ENTITIES && "Entity is out of range.");

		EntitiesTags[e] = t;
	}


	Signature EntityManager::GetSignature(Entity& e)
	{
		LOG_ASSERT(e.GetEntityID() < MAX_ENTITIES && "Entity is out of range.");

		return EntitiesSignatures[e.GetEntityID()];
	}


	Signature EntityManager::GetSignature(EntityID& e)
	{
		LOG_ASSERT(e < MAX_ENTITIES && "Entity is out of range.");

		return EntitiesSignatures[e];
	}


	Tag EntityManager::GetTag(EntityID e)
	{
		LOG_ASSERT(e < MAX_ENTITIES && "Entity is out of range.");

		return EntitiesTags[e];
	}

} // end of namespace

RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace Engine;
	registration::class_<Entity>("0Entity")
		.constructor<>()
		.property("name", &Entity::GetEntityName, &Entity::SetEntityName)
		.property("prefab", &Entity::GetPrefab, &Entity::SetPrefab)
		.property("tag", &Entity::GetKeyTag, &Entity::SetKeyTag)
		.property("parent", &Entity::GetParent, &Entity::SetParentID)
		.property("isActive", &Entity::GetIsActive, &Entity::SetIs_Active)
		;
}