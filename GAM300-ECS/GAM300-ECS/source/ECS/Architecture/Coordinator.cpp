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

#define DUPLICATE_COMPONENTS	DUPLICATE_COMPONENT(Transform)\
								DUPLICATE_COMPONENT(Script)\
								DUPLICATE_COMPONENT(std::vector<Transform>)


#define DUPLICATE_COMPONENT(type)	if(HasComponent<type>(original))\
									{\
										type* oPtr = GetComponent<type>(original);\
										type c = *oPtr;\
										AddComponent<type>(duplicated, c);\
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


	void Coordinator::DuplicateEntity(EntityID __id__)
	{
		Entity original = *GetEntity(__id__);

		EntityID id = CreateEntity(original.GetEntityName() + "(D)");
		Entity& duplicated = *GetEntity(id);
		duplicated.SetPrefab(original.GetPrefab());

		DUPLICATE_COMPONENTS
	}


	Entity Coordinator::CreateChild(EntityID parent, const std::string& __name__)
	{
		Entity e = mEntityManager->CreateChild(parent, __name__);
		mEntities.emplace_back(e);
		mParentChild[parent].emplace_back(e.GetEntityID());
		
		return e;
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
		}
		mEntityManager->DestroyEntity(e);
		mComponentManager->DestroyEntity(e);
		mSystemManager->DestroyEntity(e);

		int index = 0;
		for (; index < mEntities.size(); ++index)
		{
			// Swaps last element with current and remove it
			if (e == mEntities[index].GetEntityID())
			{
				break;
			}
		}
		mEntities.erase(mEntities.begin() + index);

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