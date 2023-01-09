/******************************************************************************/
/*!
\file       Coordinator.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       24 August 2022
\brief
  This file contains the declaration and implementation of Coordinator class.

  The coordinator class acts as a mediator for Entity Manager, Component Manager
  and System Manager, which manages them.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "include/ECS/Architecture/Coordinator.hpp"
#include "include/Memory/FreeListAllocator.hpp"

#include "include/ECS/Component/Transform.hpp"
#include "include/ECS/System/TransformSystem.hpp"

#include <memory>


#define DUPLICATE_COMPONENTS(d, o)	DUPLICATE_COMPONENT(Transform, d, o)\
									DUPLICATE_COMPONENT(Script, d, o)\
									DUPLICATE_COMPONENT(std::vector<Transform>, d, o)


#define DUPLICATE_COMPONENT(type, d, o)	if (HasComponent<type>(o))\
										{\
											type* oPtr = GetComponent<type>(o);\
											type c = *oPtr;\
											AddComponent<type>(d, c);\
										}


namespace Engine
{
	Coordinator::~Coordinator() 
	{ 
		Destroy(); 
	};


	void Coordinator::Init()
	{
		mFreeListAllocator = new FreeListAllocator((size_t)1e8);

		mEntityManager = std::make_unique<EntityManager>();
		mComponentManager = std::make_unique<ComponentManager>();
		mSystemManager = std::make_unique<SystemManager>();

		RegisterComponents();
		RegisterSystems();
	}


	void Coordinator::Destroy()
	{
		// Free all entities before allocators
		for (auto& e : GetEntities())
		{
			mEntityManager->DestroyEntity(e);
			mComponentManager->DestroyEntity(e);
			mSystemManager->DestroyEntity(e);
		}
		mEntities.clear();
		mParentChild.clear();
		mPrefabReloading.clear();

		mComponentManager->FreeCustomAllocator();

		delete mFreeListAllocator;
	}


	void Coordinator::RegisterComponents()
	{
		// Register ALL components here
		RegisterComponent<Transform, MAX_ENTITIES>();
		RegisterComponent<Script, MAX_ENTITIES>();
		RegisterComponent<std::vector<Transform>, MAX_ENTITIES>();
	}


	void Coordinator::RegisterSystems()
	{
		// Register ALL systems and their respective signature here
		RegisterSystem<TransformSystem>();
		AssignSystemSignature<TransformSystem, Transform>(Transform{});
		AssignSystemSignature<TransformSystem, Script>(Script{});
	}


	EntityID Coordinator::CreateEntity(std::string __name__)
	{
		Entity e = mEntityManager->CreateEntity(__name__);
		mEntities.emplace_back(e);

		return e.GetEntityID();
	}


	void Coordinator::DuplicateEntity(Entity entity, EntityID parentID)
	{
		// Create new entity based on parentID (As a standalone or as a child)
		EntityID duplicated_id = entity.GetParent() < MAX_ENTITIES ? CreateChild(parentID) : CreateEntity();
		Entity& duplicated_entity = *GetEntity(duplicated_id);

		// Duplicate entity (Copy all variables + Components) based on original except name
		duplicated_entity.Copy(entity);
		if (std::string prefabName = duplicated_entity.GetPrefab(); prefabName != "")
		{
			AddToPrefabMap(prefabName, duplicated_id);
		}
		duplicated_entity.SetKeyTag(entity.GetKeyTag());
		Tag tag = GetTag(entity);
		SetTag(duplicated_entity, tag);

		// Getting a unique name based on prefab's entity name
		std::string entity_name = entity.GetEntityName();
		std::string duplicate_name{};

		int i = 1;
		for (int size = 0; size < mEntities.size(); ++size)
		{
			duplicate_name = entity_name + "(" + std::to_string(i) + ")";

			if (mEntities[size].GetEntityName() == duplicate_name)
			{
				// Increament till highest value, reset back loop to check again.
				++i;
				size = 0;
			}
		}
		duplicated_entity.SetEntityName(duplicate_name);

		// Duplicate components
		DUPLICATE_COMPONENTS(duplicated_entity, entity)

		// Loop original entity children
		if (mParentChild.find(entity.GetEntityID()) != mParentChild.end())
		{
			for (auto& child : mParentChild[entity.GetEntityID()])
			{
				DuplicateEntity(*GetEntity(child), duplicated_id);
			}
		}
	}


	EntityID Coordinator::CreateChild(EntityID parent, const std::string& __name__)
	{
		(*GetEntity(parent)).SetIs_Parent(true);

		Entity e = mEntityManager->CreateChild(parent, __name__);
		mEntities.emplace_back(e); //insert this
		mParentChild[parent].emplace_back(e.GetEntityID());

		return e.GetEntityID();
	}


	void Coordinator::ToChild(EntityID parent, EntityID child)
	{
		Entity& e = *GetEntity(child);
		(*GetEntity(parent)).SetIs_Parent(true);

		if (e.IsChild())
		{
			// Remove child from the vector in mParentChild
			std::vector<EntityID>& childs = mParentChild[e.GetParent()];
			childs.erase(std::find(childs.begin(), childs.end(), child));

			if (mParentChild[e.GetParent()].size() == 0)
			{
				GetEntity(e.GetParent())->SetIs_Parent(false);
				mParentChild.erase(e.GetParent());
			}
		}
		else
		{
			e.SetIs_Child(true);
		}
		e.SetParentID(parent);

		// Add child to mParentChild
		mParentChild[parent].emplace_back(child);
	}


	void Coordinator::UnChild(EntityID parent, EntityID child)
	{
		Entity* entity_child = GetEntity(child);
		entity_child->SetIs_Child(false);
		entity_child->SetParentID(MAX_ENTITIES + 1);

		std::vector<EntityID>& children = mParentChild[parent];
		auto itr = std::find(children.begin(), children.end(), child);
		if (itr != children.end())
		{
			mParentChild[parent].erase(itr);
		}

		// If there is no more children, get rid of parent ID in map
		if (mParentChild[parent].size() == 0)
		{
			Entity* entity_parent = GetEntity(parent);
			if (entity_parent)
			{
				entity_parent->SetIs_Parent(false);
			}
			mParentChild.erase(parent);
		}
	}


	std::vector<EntityID> Coordinator::GetChildObjects(EntityID id)
	{
		auto it = mParentChild.find(id);
		if (it != mParentChild.end())
		{
			return mParentChild[id];
		}
		// Else return empty vector
		return std::vector<EntityID>();
	}


	Entity* Coordinator::GetChildObject(EntityID parent, uint32_t index)
	{
		auto children = GetChildObjects(parent);
		LOG_ASSERT(!children.empty() && "Calling GetChildObject on entity without a child!");

		return GetEntity(children[index]);
	}


	void Coordinator::GetAllChildren(std::vector<EntityID>& container, EntityID id)
	{
		container.emplace_back(id);

		for (auto& i : GetChildObjects(id))
		{
			GetAllChildren(container, i);
		}
	}


	std::vector<Entity>& Coordinator::GetEntities()
	{
		return mEntities;
	}


	std::map<EntityID, std::vector<EntityID>>& Coordinator::GetMap()
	{
		return mParentChild;
	}


	std::vector<EntityID>& Coordinator::GetPrefabContainer(std::string prefabName)
	{
		return mPrefabReloading[prefabName];
	}


	void Coordinator::UntagPrefab(EntityID id)
	{
		// Reset Prefab Name and remove from container

		std::string prefabName = GetEntity(id)->GetPrefab();
		GetEntity(id)->SetPrefab("");

		RemoveFromPrefabMap(prefabName, id);
	}


	void Coordinator::AddToPrefabMap(std::string prefabName, EntityID id)
	{
		std::vector<EntityID>& value = mPrefabReloading[prefabName];
		
		// To avoid same id being added twice
		for (int i = 0; i < value.size(); ++i)
		{
			if (id == value[i])
			{
				return;
			}
		}

		value.emplace_back(id);
	}
	

	void Coordinator::RemoveFromPrefabMap(std::string prefabName, EntityID id)
	{
		std::vector<EntityID>& value = mPrefabReloading[prefabName];

		int index = MAX_ENTITIES + 1;
		for (int i = 0; i < value.size(); ++i)
		{
			if (id == value[i])
			{
				index = i;
				break;
			}
		}

		if (index < MAX_ENTITIES)
		{
			value.erase(value.begin() + index);
		}
	}


	void Coordinator::DestroyEntity(Entity& e)
	{
		DestroyEntity(e.GetEntityID());
	}


	void Coordinator::DestroyEntity(EntityID e)
	{
		// Recursive, DFS to first child
		for (auto& child : GetChildObjects(e))
		{
			DestroyEntity(child);
		}

		Entity& entity = *GetEntity(e);

		// If valid prefabName, remove from prefabName-EntityID container
		std::string prefabName = entity.GetPrefab();
		RemoveFromPrefabMap(prefabName, e);

		// If valid parentID, remove from parent-child container
		EntityID parentID = entity.GetParent();
		if (parentID < MAX_ENTITIES)
		{
			std::vector<EntityID>& children = mParentChild[parentID];
			auto itr = std::find(children.begin(), children.end(), e);
			if (itr != children.end())
			{
				mParentChild[parentID].erase(itr);
			}

			// If there is no more children, get rid of parentID in map
			if (mParentChild[parentID].size() == 0)
			{
				Entity* entity_parent = GetEntity(parentID);
				if (entity_parent)
				{
					entity_parent->SetIs_Parent(false);
				}
				mParentChild.erase(parentID);
			}
		}

		// Remove entity from mEntities container
		int index = 0;
		for (int i = 0; i < mEntities.size(); ++i)
		{
			if (e == mEntities[i].GetEntityID())
			{
				index = i;
				break;
			}
		}
		mEntities.erase(mEntities.begin() + index);

		mEntityManager->DestroyEntity(e);
		mComponentManager->DestroyEntity(e);
		mSystemManager->DestroyEntity(e);
	}


	void Coordinator::DestroyEntity(Entity e, float delay)
	{
		v_timed_destroys.emplace_back(std::pair<unsigned int, float>(e.GetEntityID(), delay));
	}


	Entity* Coordinator::GetEntity(EntityID id)
	{
		for (int i = 0; i < mEntities.size(); ++i)
		{
			if (mEntities[i].GetEntityID() == id)
			{
				return &mEntities[i];
			}
		}
		return nullptr;
	}


	Entity* Coordinator::GetEntityByName(std::string name)
	{
		for (int i = 0; i < mEntities.size(); ++i)
		{
			if (mEntities[i].GetEntityName() == name)
			{
				return &mEntities[i];
			}
		}
		return nullptr;
	}


	bool Coordinator::IsNameRepeated(std::string name)
	{
		for (int i = 0; i < mEntities.size(); ++i)
		{
			if (name == mEntities[i].GetEntityName())
			{
				return true;
			}
		}
		return false;
	}


	bool Coordinator::EntityExists(EntityID id)
	{
		for (int i = 0; i < mEntities.size(); ++i)
		{
			if (id == mEntities[i].GetEntityID())
			{
				return true;
			}
		}
		return false;
	}


	bool Coordinator::CheckParentIsActive(Entity& e)
	{
		if (e.GetIsActive())
		{
			EntityID parent_id = e.GetParent();
			if (parent_id < MAX_ENTITIES)
			{
				Entity& parent = *GetEntity(parent_id);
				return CheckParentIsActive(parent);
			}
			else
			{
				return e.GetIsActive();
			}
		}
		else
		{
			return false;
		}
	}


	bool Coordinator::CheckParentIsActive(EntityID id)
	{
		Entity& e = *GetEntity(id);
		return CheckParentIsActive(e);
	}


	void Coordinator::SetTag(Entity& e, Tag t)
	{
		SetTag(e.GetEntityID(), t);
	}


	void Coordinator::SetTag(EntityID e, Tag t)
	{
		mEntityManager->SetTag(e, t);
	}


	Tag Coordinator::GetTag(Entity& e)
	{
		return GetTag(e.GetEntityID());
	}


	Tag Coordinator::GetTag(EntityID e)
	{
		return mEntityManager->GetTag(e);
	}

} // end of namespace