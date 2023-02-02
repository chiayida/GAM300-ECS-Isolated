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
		glm::vec4 color{1.f, 1.f, 1.f, 1.f};
		float angle{};
		float lifeRemaining = -1.f, lifespan = -1.f;
		bool isActive = false;
	};


	class Particle : public IComponent
	{
	public:
		Particle() = default;
		Particle(bool isLooping, bool isRotate, int maxParticles, float gravityModifier, float radius, float rotationSpeed,
			glm::vec4 startColor, glm::vec4 endColor, glm::vec3 minSpeed, glm::vec3 maxSpeed, glm::vec3 minSize, glm::vec3 maxSize, 
			float minLifespan, float maxLifespan, bool isSphere, bool isCone);

		void addParticle(glm::vec3 positionEntity);
		void resetParticle();
		void Init(glm::vec3 positionEntity);
		void Update(float deltaTime, glm::vec3 positionEntity);

		// Texture properties
		std::string textureName;
		GLuint texobj_hdl{};
		glm::vec2 minUV, maxUV;

		glm::vec4 startColor = { 1.f, 1.f, 1.f, 1.f }, endColor = { 1.f, 1.f, 1.f, 1.f };
		glm::vec3 minSpeed, maxSpeed;
		glm::vec3 minSize, maxSize;
		int maxParticles;
		float gravityModifier, radius, rotationSpeed;
		float minLifespan, maxLifespan;

		bool isLooping, isRotate;
		bool isCone, isSphere;

		std::array<ParticleProps, 200> particles;
		std::queue<int> availableParticles; // Pooling

		RTTR_ENABLE(IComponent);
	};
}