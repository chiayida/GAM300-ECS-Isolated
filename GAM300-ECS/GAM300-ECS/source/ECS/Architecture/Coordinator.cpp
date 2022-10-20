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
		
		return e;
	}


	void Coordinator::DestroyEntity(Entity& e)
	{
		mEntityManager->DestroyEntity(e);
		mComponentManager->DestroyEntity(e);
		mSystemManager->DestroyEntity(e);


		int index = 0;
		for (; index < mEntities.size(); ++index)
		{
			// Swaps last element with current and remove it
			if (e.GetEntityID() == mEntities[index].GetEntityID())
			{
				break;
			}
		}

		mEntities.erase(mEntities.begin() + index);
	}


	void Coordinator::DestroyEntity(EntityID e)
	{
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

} // end of namespace