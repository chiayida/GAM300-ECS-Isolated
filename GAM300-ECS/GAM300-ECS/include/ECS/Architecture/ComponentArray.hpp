/******************************************************************************/
/*!
\file       ComponentArray.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       18 August 2022
\brief
  This file contains the declaration and implementation of IComponentArray and 
  ComponentArray class.

  IComponentArray is used as an interface for ComponentManager class. It is to
  tell a generic ComponentArray that an entity has been destroyed and that it
  needs to update its array mappings.

  ComponentArray is a data structure that is always packed (no gaps).
  - Array should be packed so that we are able to iterate through the array 
  without any "If (valid)" checks.

  To prevent stale data with no entity attached, it is mapped from entity IDs 
  to array indices:
  - When accessing the array, it uses the entity ID to look up the actual array
  index. 
  - When an entity is destroyed, it takes the last valid element in the
  array and move into deleted entity's spot and updates the map.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "ECS.hpp"
#include "EntityManager.hpp"
#include "include/Memory/CustomAllocator.hpp"
#include "include/Logging.hpp"
#include <unordered_map>

namespace Engine
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity& e) = 0;
		virtual void EntityDestroyed(EntityID& e) = 0;
		virtual void FreeComponentArray() = 0;
	};


	// T - Component Type
	// N - Number of elements per chunk.
	template <typename T, unsigned N>
	class ComponentArray : public IComponentArray
	{
	public:
		ComponentArray(Allocator* allocator_);

		template <typename... argv>
		bool AddComponent(Entity& e, argv... args);
		template <typename... argv>
		bool AddComponent(EntityID& e, argv... args);

		void RemoveComponent(Entity& e);
		void RemoveComponent(EntityID& e);

		bool HasData(Entity& e);
		bool HasData(EntityID& e);

		T* GetData(const Entity& e);
		T* GetData(EntityID& e);

		void EntityDestroyed(Entity& e) override;
		void EntityDestroyed(EntityID& e) override;
		void FreeComponentArray() override;

	private:
		// Packed array of T component.
		CustomAllocator<T, N>* mComponentArrayAllocator = nullptr;
		std::unordered_map<EntityID, T*> EntityComponentMap{};
	};


	// Templated class functions implementations

	template <typename T, unsigned N>
	ComponentArray<T, N>::ComponentArray(Allocator* allocator_)
	{
		mComponentArrayAllocator = new CustomAllocator<T, N>(allocator_);
	}


	template <typename T, unsigned N>
	template <typename... argv>
	bool ComponentArray<T, N>::AddComponent(Entity& e, argv... args)
	{
		if (EntityComponentMap.find(e.GetEntityID()) != EntityComponentMap.end())
		{
			LOG_WARNING("Repeated component added to same entity.");
			return false;
		}

		T* component = mComponentArrayAllocator->Allocate(args ...);
		if (component == nullptr)
		{
			return false;
		}

		EntityComponentMap[e.GetEntityID()] = component;
		return true;
	}


	template <typename T, unsigned N>
	template <typename... argv>
	bool ComponentArray<T, N>::AddComponent(EntityID& e, argv... args)
	{
		if (EntityComponentMap.find(e) != EntityComponentMap.end())
		{
			LOG_WARNING("Repeated component added to same entity.");
			return false;
		}

		T* component = mComponentArrayAllocator->Allocate(args ...);
		if (component == nullptr)
		{
			return false;
		}

		EntityComponentMap[e] = component;
		return true;
	}


	template <typename T, unsigned N>
	void ComponentArray<T, N>::RemoveComponent(Entity& e)
	{
		if (EntityComponentMap.find(e.GetEntityID()) == EntityComponentMap.end())
		{
			LOG_WARNING("Removing non-existent component.");
			return;
		}

		T* component = GetData(e);
		EntityComponentMap.erase(e.GetEntityID());
		mComponentArrayAllocator->Free(component);
	}


	template <typename T, unsigned N>
	void ComponentArray<T, N>::RemoveComponent(EntityID& e)
	{
		if (EntityComponentMap.find(e) == EntityComponentMap.end())
		{
			LOG_WARNING("Removing non-existent component.");
			return;
		}

		T* component = GetData(e);
		EntityComponentMap.erase(e);
		mComponentArrayAllocator->Free(component);
	}


	template <typename T, unsigned N>
	bool ComponentArray<T, N>::HasData(Entity& e)
	{
		if (EntityComponentMap.find(e.GetEntityID()) == EntityComponentMap.end())
		{
			return false;
		}

		return true;
	}


	template <typename T, unsigned N>
	bool ComponentArray<T, N>::HasData(EntityID& e)
	{
		if (EntityComponentMap.find(e) == EntityComponentMap.end())
		{
			return false;
		}

		return true;
	}


	template <typename T, unsigned N>
	T* ComponentArray<T, N>::GetData(const Entity& e)
	{
		if (EntityComponentMap.find(e.GetEntityID()) == EntityComponentMap.end())
		{
			LOG_WARNING("Retrieving non-existent component.");
			return nullptr;
		}

		return EntityComponentMap[e.GetEntityID()];
	}


	template <typename T, unsigned N>
	T* ComponentArray<T, N>::GetData(EntityID& e)
	{
		if (EntityComponentMap.find(e) == EntityComponentMap.end())
		{
			LOG_WARNING("Retrieving non-existent component.");
			return nullptr;
		}

		return EntityComponentMap[e];
	}


	template <typename T, unsigned N>
	void ComponentArray<T, N>::EntityDestroyed(Entity& e)
	{
		if (EntityComponentMap.find(e.GetEntityID()) != EntityComponentMap.end())
		{
			RemoveComponent(e);
		}
	}


	template <typename T, unsigned N>
	void ComponentArray<T, N>::EntityDestroyed(EntityID& e)
	{
		if (EntityComponentMap.find(e) != EntityComponentMap.end())
		{
			RemoveComponent(e);
		}
	}


	template <typename T, unsigned N>
	void ComponentArray<T, N>::FreeComponentArray()
	{
		if (mComponentArrayAllocator)
		{
			delete mComponentArrayAllocator;
			mComponentArrayAllocator = nullptr;
		}
	}

} // end of namespace