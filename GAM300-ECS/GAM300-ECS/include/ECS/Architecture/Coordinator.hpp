/******************************************************************************/
/*!
\file       Coordinator.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       24 August 2022
\brief
  This file contains the declaration and implementation of Coordinator class.

  The coordinator class acts as a mediator for Entity Manager, Component Manager
  and System Manager, which manages them.

  This enables us to have a single instance of the coordinator which we then can
  use it to interface with the other managers.

  
  Sequence of using the coordinator (Coordinator gCoordinator):
  1) Coordinator gCoordinator;	gCoordinator.Init();
  - Initialise Entity Manager, Component Manager, System Manager.
  - Register Component and System classes and systems within .Init().
  - You can set a cap on the number of a particular Component.
  - Setting of System's Signature is also under .Init().

  2) std::shared_ptr<System> ptrPhysicsSystem = gCoordinator.GetSystem<PhysicsSystem>();
  - After registering system, you can call the Init/Update/Destroy functions of the system

  3) gCoordinator.CreateEntity();
  - To create an entity.
  - Get the ID of the entity with .GetEntityID();
  - Get the name of the entity with .GetEntityName();
  - Set the name of the entity with .SetEntityName(std::string __name__);

  4) gCoordinator.AddComponent<T>(entity, ...);
  - Tag component to entity, T is Component class, ... is component constructor params

  5) gCoordinator.RemoveComponent<T>(entity);
  - Tag component to entity, T is Component class

  6) gCoordinator.Destroy();
  - Free Coordinator's allocated memory.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "ECS.hpp"
#include "include/Memory/Allocator.hpp"
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

#define UNUSED(expr) (void)expr

namespace Engine
{
	class Coordinator
	{
	public:
		~Coordinator();
		void Init();
		void InitForLoading();
		void Destroy();

		void RegisterComponents(); // Function is to register all components
		void RegisterSystems();	   // Function is to register all systems

		void EndOfLoopUpdate(); // Function is to update at the end (Scripts)

		// Creates an entity and returns the ID
		EntityID CreateEntity(std::string __name__ = "");
		void DuplicateEntity(Entity entity, EntityID parentID);

		// Creates a child entity and returns the ID
		EntityID CreateChild(EntityID parent, const std::string&__name__ = "");

		// Convert the entity (child) into a child passing in the parent-to-be (parent)
		void ToChild(EntityID parent, EntityID child);
		void UnChild(EntityID parent, EntityID child);

		// Get a vector of the child objects based on the entity ID
		std::vector<EntityID> GetChildObjects(EntityID id);
		Entity *GetChildObject(EntityID parent, uint32_t index = 0);

		std::map<EntityID, std::vector<EntityID>>& GetMap();

		// Resets entity's signature, remove it from component arrays and systems
		void DestroyEntity(Entity &e);
		void DestroyEntity(EntityID e);
		void DestroyEntity(Entity e, float delay);

		// Tag component to entity, update signature in EntityManager, SystemManager System's entities
		template <typename T, unsigned N = 1, typename... argv>
		void AddComponent(Entity &e, argv... args);
		template <typename T, unsigned N = 1, typename... argv>
		void AddComponent(EntityID e, argv... args);

		// Remove tagged component from entity, update signature in EntityManager and System's entities
		template <typename T, unsigned N = 0>
		void RemoveComponent(Entity &e);
		template <typename T, unsigned N = 0>
		void RemoveComponent(EntityID e);

		// Checks whether entity has a component
		template <typename T>
		bool HasComponent(Entity &e);
		template <typename T>
		bool HasComponent(EntityID e);

		// Retrieve component from entity (For updating of component's variables)
		// T& GetComponent(EntityID e);
		template <typename T>
		T* GetComponent(const Entity& e);
		template <typename T>
		T *GetComponent(EntityID e);

		// Retrieve pointer of system using type.
		template <typename T>
		std::shared_ptr<System> GetSystem();

		Entity* GetEntity(EntityID id);
		Entity* GetEntityByName(std::string name);
		std::vector<Entity>& GetEntities();

		bool IsNameRepeated(std::string name);

		bool EntityExists(Entity const& ent) const;
	private:
		/* Member Functions */

		// Register component as a component array
		template <typename T, unsigned N>
		void RegisterComponent();

		// Retrieve component's type (To create signature for system)
		template <typename T>
		ComponentType GetComponentType();

		// Register class as a System
		template <typename T>
		void RegisterSystem();
		// std::shared_ptr<T> RegisterSystem();

		// Variadic templated function to assign components to System's signature
		template <typename T, typename... argv>
		void AssignSystemSignature(argv... args);

		// Variadic templated function for assigning System's signature (To be looped)
		template <typename T, typename... argv>
		void AssignSystemSig(Signature &s, T var1, argv... args);

		// Variadic templated function for assigning System's signature (Last loop)
		template <typename T>
		void AssignSystemSig(Signature &s, T var1);

		// Tag signature to system
		template <typename T>
		void SetSystemSignature(Signature signature);

		/* Data Members */
		Allocator *mFreeListAllocator = nullptr;

		std::unique_ptr<EntityManager> mEntityManager;
		std::unique_ptr<ComponentManager> mComponentManager;
		std::unique_ptr<SystemManager> mSystemManager;

		std::vector<Entity> mEntities{};
		// 1 level of child
		std::map<EntityID, std::vector<EntityID>> mParentChild;

		// Timed Destroy (Scripts)
		std::vector<std::pair<unsigned int, float>> v_timed_destroys;
	};

	// Templated class functions implementations


	template <typename T, unsigned N, typename... argv>
	void Coordinator::AddComponent(Entity &e, argv... args)
	{
		mComponentManager->AddComponent<T, N>(e, args...);

		// Update Entity's signature
		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(e, signature);

		// Update System's entity container
		mSystemManager->EntitySignatureChanged(e, signature);
	}


	template <typename T, unsigned N, typename... argv>
	void Coordinator::AddComponent(EntityID e, argv... args)
	{
		mComponentManager->AddComponent<T, N>(e, args...);

		// Update Entity's signature
		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(e, signature);

		// Update System's entity container
		mSystemManager->EntitySignatureChanged(e, signature);
	}


	template <typename T, unsigned N>
	void Coordinator::RemoveComponent(Entity &e)
	{
		mComponentManager->RemoveComponent<T, N>(e);

		// Update Entity's signature
		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(e, signature);

		// Update System's entity container
		mSystemManager->EntitySignatureChanged(e, signature);
	}


	template <typename T, unsigned N>
	void Coordinator::RemoveComponent(EntityID e)
	{
		mComponentManager->RemoveComponent<T, N>(e);

		// Update Entity's signature
		auto signature = mEntityManager->GetSignature(e);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(e, signature);

		// Update System's entity container
		mSystemManager->EntitySignatureChanged(e, signature);
	}


	template <typename T>
	bool Coordinator::HasComponent(Entity &e)
	{
		return mComponentManager->HasComponent<T>(e);
	}


	template <typename T>
	bool Coordinator::HasComponent(EntityID e)
	{
		return mComponentManager->HasComponent<T>(e);
	}


	//T& Coordinator::GetComponent(EntityID e)
	template<typename T>
	T* Coordinator::GetComponent(const Entity& e)
	{
		return mComponentManager->GetComponent<T>(e);
	}


	//T& Coordinator::GetComponent(EntityID e)
	template<typename T>
	T* Coordinator::GetComponent(EntityID e)
	{
		return mComponentManager->GetComponent<T>(e);
	}


	template <typename T>
	std::shared_ptr<System> Coordinator::GetSystem()
	{
		return mSystemManager->GetSystem<T>();
	}


	template <typename T, unsigned N>
	void Coordinator::RegisterComponent()
	{
		mComponentManager->RegisterComponent<T, N>(mFreeListAllocator);
	}


	template <typename T>
	ComponentType Coordinator::GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}


	// std::shared_ptr<T> Coordinator::RegisterSystem()
	template <typename T>
	void Coordinator::RegisterSystem()
	{
		mSystemManager->RegisterSystem<T>();
	}


	template <typename T, typename... argv>
	void Coordinator::AssignSystemSignature(argv... args)
	{
		Signature signature;

		AssignSystemSig(signature, args...);
		SetSystemSignature<T>(signature);
	}


	template <typename T, typename... argv>
	void Coordinator::AssignSystemSig(Signature &s, T var1, argv... args)
	{
		UNUSED(var1);

		s.set(GetComponentType<T>());
		AssignSystemSig(s, args...);
	}


	template <typename T>
	void Coordinator::AssignSystemSig(Signature &s, T var1)
	{
		UNUSED(var1);

		s.set(GetComponentType<T>());
	}


	template <typename T>
	void Coordinator::SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSignature<T>(signature);
	}

} // end of namespace