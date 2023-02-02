/******************************************************************************/
/*!
\file       Particle.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       16 January 2023
\brief
  This file contains the declaration of Particle class.

  Fields can be adjusted freely, particles will be randomized
  from min to max variables.
  If you want it to be constant, set the min and max to be the same values.

  maxParticles is how many particles are "alive" at the same time, per frame.

  isCone, isSphere is the flags for emission shapes.
  Emission shape = spawn particles within that shape. NOT MOVE in that shape. (Unity)

  Init should be called BEFORE the setter function for isLooping.

  Pooling is for reusing the particles.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
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

		// Pooling
		std::array<ParticleProps, 200> particles;
		std::queue<int> availableParticles;

		RTTR_ENABLE(IComponent);
	};
}