/******************************************************************************/
/*!
\file       SystemManager.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       19 August 2022
\brief
  This file contains the declaration and implementation of SystemManager classes.

  The SystemManager class is responsible of maintaining a record of registered
  systems and their signatures.

  Each system needs to have a signature set so that the manager can add the
  appropriate entities to each system's std::set of entities.

  std::set of entities should be updated whenever the entity's signature is updated.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "ECS.hpp"
#include "System.hpp"
#include "EntityManager.hpp"
#include "include/Logging.hpp"

#include <set>
#include <memory>
#include <unordered_map>

namespace Engine
{
	class SystemManager
	{
	public:
		template<typename T>
		void RegisterSystem();

		template<typename T>
		void SetSignature(Signature signature);

		void DestroyEntity(Entity& e);
		void DestroyEntity(EntityID& e);

		void EntitySignatureChanged(Entity& e, Signature signature);
		void EntitySignatureChanged(EntityID& e, Signature signature);

		template <typename T>
		std::shared_ptr<System> GetSystem();

	private:
		// Map system name to signature
		std::unordered_map<const char*, Signature> mSignatures{};

		// Map system name to system pointer
		std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
	};


	// Templated functions implementations

	template<typename T>
	void SystemManager::RegisterSystem()
	{
		const char* name = typeid(T).name();

		//assert(mSystems.find(name) == mSystems.end() && "Registering system more than once.");
		if (mSystems.find(name) != mSystems.end())
		{
			LOG_WARNING("Registering system more than once.");
			return;
		}

		// Create a system pointer and add it to map
		auto system = std::make_shared<T>();
		mSystems.insert({ name, system });
	}


	template<typename T>
	void SystemManager::SetSignature(Signature signature)
	{
		const char* name = typeid(T).name();

		if (mSystems.find(name) == mSystems.end())
		{
			LOG_WARNING("System is not registered.");
			return;
		}

		// Set signature
		mSignatures.insert({ name, signature });
	}


	template <typename T>
	std::shared_ptr<System> SystemManager::GetSystem()
	{
		const char* name = typeid(T).name();

		if (mSystems.find(name) == mSystems.end())
		{
			LOG_WARNING("System is not registered.");
			return nullptr;
		}

		return mSystems.find(name)->second;
	}

} // end of namespace