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
  
  - Layer, MAX_LAYERS, Tag 
  (A mock of unity's physics tag and layer, catered to what is needed ONLY)

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
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

	using Layer = std::uint8_t;
	const Layer MAX_LAYERS = 32;
	using Tag = std::bitset<MAX_LAYERS>;

} // end of namespace