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


#define DUPLICATE_COMPONENT(type, d, o)	if(HasComponent<type>(o))\
										{\
											type* oPtr = GetComponent<type>(o);\
											type c = *oPtr;\
											AddComponent<type>(d, c);\
										}

// Only for colliders as PhyX needs Actor
#define DUPLICATE_COMPONENT_COLLIDER(type, d, o)	if(HasComponent<type>(o) && HasComponent<Transform>(o))\
													{\
														type* oPtr = GetComponent<type>(o);\
														AddComponent<type>(d, type(GetComponent<Transform>(d)));\
														type* dPtr= GetComponent<type>(d);\
														*dPtr = *oPtr;\
														dPtr->CreateActor();\
														dPtr->AddActor();\
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


	void Coordinator::InitForLoading()
	{
		mFreeListAllocator = new FreeListAllocator((size_t)1e8);

		mEntityManager = std::make_unique<EntityManager>();
		mComponentManager = std::make_unique<ComponentManager>();
		mSystemManager = std::make_unique<SystemManager>();

		//using namespace Graphics::Addition;
		//RegisterComponent<FullMesh, 1>();
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
		GetEntities().clear();

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
		AssignSystemSignature<TransformSystem, std::vector<Transform>>(std::vector<Transform>{});
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

		// Duplicate entity (Copy all variables + Components) based on original
		duplicated_entity.Copy(entity);
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


	std::map<EntityID, std::vector<EntityID>>& Coordinator::GetMap()
	{
		return mParentChild;
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
		EntityID parentID = entity.GetParent();

		// If valid parentID, remove from parent-child container
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


	std::vector<Entity>& Coordinator::GetEntities()
	{
		return mEntities;
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


	bool Coordinator::EntityExists(Entity const& ent) const
	{
		for (Entity const& e : mEntities)
		{
			if (e.GetEntityID() == ent.GetEntityID())
			{
				return true;
			}
		}
		return false;
	}

} // end of namespace