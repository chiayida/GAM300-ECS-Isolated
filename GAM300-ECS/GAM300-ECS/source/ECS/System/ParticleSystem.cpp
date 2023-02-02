/******************************************************************************/
/*!
\file       ParticleSystem.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       16 January 2023
\brief
  This file contains the implementation of ParticleSystem class.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#include "include/ECS/System/ParticleSystem.hpp"
#include "include/Graphics/Mesh.hpp"
#include "include/Graphics/shader.hpp"
#include "include/Logging.hpp"

#include "include/ECS/Component/Particle.hpp"
#include "include/ECS/Architecture/Coordinator.hpp"

namespace Engine
{
	void ParticleSystem::Init()
	{
		LOG_INSTANCE("Particle System created");

		Renderer::Init();
	}


	void ParticleSystem::Update()
	{
		// Do nothing
	}

	//glm::vec3 rotationT = { 36.7f, 0.f, 0.f };
	glm::vec3 rotationT = { 0.f, 0.f, 0.f };
	glm::vec3 position = { 0.f, 0.f, -3.f };
	void ParticleSystem::Update(Coordinator* coordinator, float deltaTime)
	{
		// Load Default shader program
		const auto& shd_ref_handle = shdrpgms[GraphicShader::Default].GetHandle();
		glUseProgram(shd_ref_handle);

		Renderer::BeginCubeBatch();

		for (auto entity : mEntities)
		{
			Particle& particle = *coordinator->GetComponent<Particle>(entity);
			particle.Update(deltaTime, glm::vec3{ 0.f, 0.f, -3.f }, rotationT);

			for (int i = 0; i < particle.particles.size(); ++i)
			{
				if (particle.particles[i].isActive == true)
				{
					glm::vec3 rotation = glm::radians(rotationT);
					// X
					glm::mat4 rotationX = glm::mat4(glm::vec4(1.0, 0.0, 0.0, 0.0),
						glm::vec4(0.0, cos(rotation.x), sin(rotation.x), 0.0),
						glm::vec4(0.0, -sin(rotation.x), cos(rotation.x), 0.0),
						glm::vec4(0.0, 0.0, 0.0, 1.0));

					// Y
					glm::mat4 rotationY = glm::mat4(glm::vec4(cos(rotation.y), 0.0, -sin(rotation.y), 0.0),
						glm::vec4(0.0, 1.0, 0.0, 0.0),
						glm::vec4(sin(rotation.y), 0.0, cos(rotation.y), 0.0),
						glm::vec4(0.0, 0.0, 0.0, 1.0));

					// Z 
					glm::mat4 rotationZ = glm::mat4(glm::vec4(cos(rotation.z), sin(rotation.z), 0.0, 0.0),
						glm::vec4(-sin(rotation.z), cos(rotation.z), 0.0, 0.0),
						glm::vec4(0.0, 0.0, 1.0, 0.0),
						glm::vec4(0.0, 0.0, 0.0, 1.0));

					// Might not be the right matrix multiplication order. 
					// In my case it doesnt matter as its only for particles
					glm::mat4 rotationMatrix = rotationZ * rotationX * rotationY;

					// calculate new position based on its entity's position and rotation
					glm::vec4 v4position = rotationMatrix * glm::vec4{ particle.particles[i].position, 1.f };
					glm::vec3 v3position = position + glm::vec3{ v4position };

					Renderer::DrawCube(v3position, particle.particles[i].size, 
						particle.particles[i].angle, particle.particles[i].color);
				}
			}

			Renderer::DrawCube(position, {0.1f, 0.1f, 0.1f}, rotationT, particle.texobj_hdl);
		}

		Renderer::EndCubeBatch();
		Renderer::FlushCube();

		glUseProgram(0);
	}


	void ParticleSystem::Destroy()
	{
		Renderer::Shutdown();

		LOG_INSTANCE("Particle System destroyed");
	}
}