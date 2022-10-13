/*!*****************************************************************************
\file   Transform.hpp
\author Cruz Rolly Matthew Capiral
\par    DP email: cruzrolly.m\@digipen.edu
\date   12/9/2022

\brief
This file includes the definition of the transform component.

Copyright (C) 2022 DigiPen Institure of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#pragma once
#include <lib/glm/glm.hpp>
#include <lib/glm/gtc/quaternion.hpp>
#include "include/ECS/Architecture/IComponent.hpp"
#include "include/ECS/Architecture/EntityManager.hpp"

#include <lib/rttr/type>
#include <lib/rttr/registration.h>


namespace Engine
{
	class Transform : public IComponent
	{
	public:
		Transform(glm::vec3 const& pos = { 0.f, 0.f, 0.f }) : position{ pos } {}

		std::map<unsigned int, std::vector<unsigned int>> map{};
		glm::vec3 position = {1.f, 2.f, 3.f };
	};

	RTTR_REGISTRATION
	{
		using namespace rttr;
		using namespace Engine;

		registration::class_<IComponent>("IComponent")
			.property("IsActive", &IComponent::isActive)
			;

		registration::class_<Transform>("Transform")
			.constructor<>()
			.property("position", &Transform::position)
			.property("map", &Transform::map)
			;
	}
}
