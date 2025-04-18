/******************************************************************************/
/*!
\file       Transform.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       18 August 2022
\brief
  This file contains the declaration and implementation of Transform class.
  
  It is just for me to play around and test whatever I am implementing at the 
  moment.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <lib/glm/glm.hpp>
#include <lib/glm/gtc/quaternion.hpp>
#include "include/ECS/Architecture/IComponent.hpp"
#include "include/ECS/Architecture/EntityManager.hpp"

#include "include/ECS/Component/Particle.hpp"

#include "rttr/type.h"
#include "rttr/registration.h"

namespace Engine
{
	class Transform : public IComponent
	{
	public:
		Transform(glm::vec3 const& pos = { 0.f, 0.f, 0.f }, glm::vec3 const& scale = { 1.f, 1.f, 1.f }, 
			glm::quat const& rot = { 0.f, 0.f, 0.f, 0.f }) : position{ pos }, scale{ scale }, rot_q{ rot },
			isOverridePosition{ false }, isOverrideScale{ false }, isOverrideRotation { false } {}

		void operator+=(Transform const& rhs) 
		{ 
			position += rhs.position; 
			scale *= rhs.scale;
			rot_q *= rhs.rot_q;
		}

		glm::vec3 position;
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::quat rot_q{};

		bool isOverridePosition = false;
		bool isOverrideScale = false;
		bool isOverrideRotation = false;
		
		RTTR_ENABLE(IComponent);
	};

	RTTR_REGISTRATION
	{
		using namespace rttr;
		using namespace Engine;

		registration::class_<IComponent>("IComponent")
			.property("IsActive", &IComponent::isActive)
			;

		registration::class_<Transform>("1Transform")
			.constructor<>()
			.property("position", &Transform::position)
			.property("scale", &Transform::scale)
			.property("rot_q", &Transform::rot_q)
			.property("isOverridePosition", &Transform::isOverridePosition)
			.property("isOverrideScale", &Transform::isOverrideScale)
			.property("isOverrideRotation", &Transform::isOverrideRotation)
			;

		registration::class_<Particle>("Particle")
			.constructor<>()
			.property("textureName", &Particle::textureName)
			;
	}
}