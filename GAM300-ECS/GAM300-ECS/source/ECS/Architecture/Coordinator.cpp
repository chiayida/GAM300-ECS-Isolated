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


	void Coordinator::DuplicateEntity(EntityID o_id, EntityID d_id)
	{
		Entity original = *GetEntity(o_id);

		// Highest level/ Top of "tree", parent and not a child
		if (original.isParent() && !original.IsChild())
		{
			// Create and duplicate entity
			EntityID id = CreateEntity();
			Entity& duplicated = *GetEntity(id);
			duplicated.Copy(original);
			DUPLICATE_COMPONENTS(duplicated, original)

			// Get children from original
			for (auto& child : mParentChild[o_id])
			{
				EntityID c_id = CreateChild(id);
				Entity& c_duplicated = *GetEntity(c_id);
				Entity& c_original = *GetEntity(child);
				c_duplicated.Copy(c_original);
				DUPLICATE_COMPONENTS(c_duplicated, c_original)

				// If it is a parent/child
				if (c_original.isParent())
				{
					DuplicateEntity(child, c_id);
				}
			}
		}
		// If it is a parent/child (To eliminate creating of same entity)
		else if (original.isParent() && original.IsChild())
		{
			// Get children from original
			for (auto& child : mParentChild[o_id])
			{
				// Create child with duplicated's parent id
				EntityID c_id = CreateChild(d_id);
				Entity& c_duplicated = *GetEntity(c_id);
				Entity& c_original = *GetEntity(child);
				c_duplicated.Copy(c_original);
				DUPLICATE_COMPONENTS(c_duplicated, c_original)

				// If it is a parent/child
				if (c_original.isParent())
				{
					DuplicateEntity(child, c_id);
				}
			}
		}
		// Basic entity without parent or child
		else
		{
			// Create and duplicate entity
			EntityID id = CreateEntity();
			Entity& duplicated = *GetEntity(id);
			duplicated.Copy(original);
			DUPLICATE_COMPONENTS(duplicated, original)
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
		Entity* ent = GetEntity(e);

		// Delete child id from parent's vector
		if (ent && ent->IsChild())
		{
			std::vector<EntityID>& children = mParentChild[ent->GetParent()];
			auto child = std::find(children.begin(), children.end(), ent->GetEntityID());
			if (child != children.end())
			{
				mParentChild[ent->GetParent()].erase(child);
			}

			if (mParentChild[ent->GetParent()].size() == 0)
			{
				GetEntity(ent->GetParent())->SetIs_Parent(false);
				mParentChild.erase(ent->GetParent());
			}
		}

		// Recursively delete all child entities
		std::map<EntityID, std::vector<EntityID>>::iterator it = mParentChild.find(e);
		if (it != mParentChild.end())
		{
			for (EntityID child : it->second)
			{
				DestroyEntity(child);
			}
			mParentChild.erase(e);
		}

		// Remove from mEntities container
		int index = 0;
		for (int i = 0; i < mEntities.size(); ++i)
		{
			//std::cout << "entity id to be deleted: " << e << std::endl;

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