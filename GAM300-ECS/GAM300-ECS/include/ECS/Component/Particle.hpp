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

#include "rttr/type.h"
#include "rttr/registration.h"

namespace Engine
{
	class ParticleProps
	{
	public:
		glm::vec3 position{};
		glm::vec3 velocity{};
		float lifespan{};
	};


	class Particle : public IComponent
	{
	public:
		Particle(int maxParticles) : minUV{ 0.f, 0.f }, maxUV{ 1.f, 1.f } 
		{
			this->maxParticles = maxParticles;

			// Initialise an array based on maxParticles
			particles = new ParticleProps[maxParticles];
			for (int i = 0; i < maxParticles; ++i)
			{
				particles[i].lifespan = -1;
			}
		}


		void Update(float deltaTime)
		{
			for (int i = 0; i < maxParticles; ++i)
			{
				if (particles[i].lifespan > 0)
				{
					particles[i].position += particles[i].velocity * deltaTime;
					particles[i].lifespan -= deltaTime;
				}
			}
		}


		// Texture properties
		std::string textureName;
		GLuint texobj_hdl{};
		glm::vec2 minUV, maxUV;

		int maxParticles;
		ParticleProps* particles;

		RTTR_ENABLE(IComponent);
	};
}