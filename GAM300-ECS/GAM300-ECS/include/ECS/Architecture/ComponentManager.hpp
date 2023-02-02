/******************************************************************************/
/*!
\file       ComponentManager.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       19 August 2022
\brief
  This file contains the declaration and implementation of ComponentManager class.

  ComponentManager is in charge of handling the different ComponentArrays when 
  a component is added or removed. 

  It contains a ComponentType variable that increments by one whenever a component
  is registered.

  It also have a unique key which is a map to ComponentArray to the ComponentType

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "ECS.hpp"
#include "EntityManager.hpp"
#include "ComponentArray.hpp"

#include <unordered_map>
#include <memory>

namespace Engine
{
	class ComponentManager
	{
	public:
		template<typename T, unsigned N>
		void RegisterComponent(Allocator* allocator_);

		template<typename T>
		ComponentType GetComponentType();

		template<typename T, unsigned N, typename... argv>
		bool AddComponent(Entity& e, argv... args);
		template<typename T, unsigned N, typename... argv>
		bool AddComponent(EntityID& e, argv... args);

		template<typename T, unsigned N>
		void RemoveComponent(Entity& e);
		template<typename T, unsigned N>
		void RemoveComponent(EntityID& e);

		template<typename T, unsigned N = 1>
		bool HasComponent(Entity& e);
		template<typename T, unsigned N = 1>
		bool HasComponent(EntityID& e);

		template<typename T, unsigned N = 1>
		T* GetComponent(const Entity& e);
		template<typename T, unsigned N = 1>
		T* GetComponent(EntityID& e);

		void DestroyEntity(Entity& e);
		void DestroyEntity(EntityID& e);

		void FreeCustomAllocator();

	private:
		// Get static pointer to ComponentArray of type T.
		template<typename T, unsigned N>
		std::shared_ptr<ComponentArray<T, N>> GetComponentArray();

		// Map type name to component type
		std::unordered_map<const char*, ComponentType> mComponentTypes{};

		// Map type name to component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

		// Component type to be assigned to the next registered component
		ComponentType NextComponentType{};
	};


	// Templated functions implementations

	template <typename T, unsigned N>
	std::shared_ptr<ComponentArray<T, N>> ComponentManager::GetComponentArray()
	{
		const char* name = typeid(T).name();

		if (mComponentTypes.find(name) == mComponentTypes.end())
		{
			LOG_WARNING("Component is not registered.");
			return nullptr;
		}

		return std::static_pointer_cast<ComponentArray<T, N>>(mComponentArrays[name]);
	}


	template<typename T, unsigned N>
	void ComponentManager::RegisterComponent(Allocator* allocator_)
	{
		const char* name = typeid(T).name();

		if (mComponentTypes.find(name) != mComponentTypes.end())
		{
			LOG_WARNING("Registering component more than once.");
			return;
		}

		// Add to component type map
		mComponentTypes.insert({ name, NextComponentType });

		// Create a ComponentArray pointer and add it to the component arrays map
		mComponentArrays.insert({ name, std::make_shared<ComponentArray<T, N>>(allocator_) });

		++NextComponentType;
	}


	template<typename T>
	ComponentType ComponentManager::GetComponentType()
	{
		const char* name = typeid(T).name();

		LOG_ASSERT(mComponentTypes.find(name) != mComponentTypes.end() && "Component not registered.");

		return mComponentTypes[name];
	}


	template<typename T, unsigned N, typename... argv>
	bool ComponentManager::AddComponent(Entity& e, argv... args)
	{
		// Add component to the array
		return GetComponentArray<T, N>()->AddComponent(e, args ...);
	}


	template<typename T, unsigned N, typename... argv>
	bool ComponentManager::AddComponent(EntityID& e, argv... args)
	{
		// Add component to the array
		return GetComponentArray<T, N>()->AddComponent(e, args ...);
	}


	template<typename T, unsigned N>
	void ComponentManager::RemoveComponent(Entity& e)
	{
		// Remove component from the array
		GetComponentArray<T, N>()->RemoveComponent(e);
	}


	template<typename T, unsigned N>
	void ComponentManager::RemoveComponent(EntityID& e)
	{
		// Remove component from the array
		GetComponentArray<T, N>()->RemoveComponent(e);
	}


	template<typename T, unsigned N>
	bool ComponentManager::HasComponent(Entity& e)
	{
		// Get a reference to the component
		if (GetComponentArray<T, N>())
		{
			return GetComponentArray<T, N>()->HasData(e);
		}

		return false;
	}


	template<typename T, unsigned N>
	bool ComponentManager::HasComponent(EntityID& e)
	{
		// Get a reference to the component
		if (GetComponentArray<T, N>())
		{
			return GetComponentArray<T, N>()->HasData(e);
		}

		return false;
	}


	template<typename T, unsigned N>
	T* ComponentManager::GetComponent(const Entity& e)
	{
		// Get a reference to the component
		return GetComponentArray<T, N>()->GetData(e);
	}


	template<typename T, unsigned N>
	T* ComponentManager::GetComponent(EntityID& e)
	{
		// Get a reference to the component
		return GetComponentArray<T, N>()->GetData(e);
	}

} // end of namespace