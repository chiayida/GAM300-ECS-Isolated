/******************************************************************************/
/*!
\file       ECS.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       18 August 2022
\brief
  This file contains the definition of variables: 
  - EntityID
  - MAX_ENTITIES (Maximum amount of entities)
  - ComponentType
  - MAX_COMPONENTS (Maximum amount of component types)
  - Signature (Use for tracking in systems. Bitset.)
*/
/******************************************************************************/
#pragma once

#include <cstdint>
#include <bitset>

namespace Engine
{
	using EntityID = std::uint32_t;
	const EntityID MAX_ENTITIES = 5000;

	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;

} // end of namespace