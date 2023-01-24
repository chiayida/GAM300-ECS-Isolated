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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "include/ECS/Architecture/IComponent.hpp"
#include <queue>
#include <array>

#include "rttr/type.h"
#include "rttr/registration.h"

namespace Engine
{
	class ParticleProps
	{
	public:
		glm::vec3 position{};
		glm::vec3 velocity{};
		glm::vec3 size{};
		float lifespan = -1.f;
	};


	class Particle : public IComponent
	{
	public:
		Particle() = default;
		Particle(bool isLooping, int particlesPerEmission,
			float minSpeed, float maxSpeed, float minSize, float maxSize, float minLifespan, float maxLifespan,
			bool isCone, float coneRadius, float coneRadiusRange, float coneAngle, bool isSphere, float sphereRadius, bool isBox);

		void addParticle(glm::vec3 positionEntity);
		void Update(float deltaTime, glm::vec3 positionEntity);

		// Texture properties
		std::string textureName;
		GLuint texobj_hdl{};
		glm::vec2 minUV, maxUV;

		float minSpeed, maxSpeed;
		float minSize, maxSize;
		float minLifespan, maxLifespan;

		float coneRadius, coneRadiusRange, coneAngle;
		float sphereRadius;

		bool isLooping;
		bool isCone, isBox, isSphere;

		int particlesPerEmission;
		std::array<ParticleProps, 200> particles;
		std::queue<int> availableParticles; // Pooling

		RTTR_ENABLE(IComponent);
	};
}