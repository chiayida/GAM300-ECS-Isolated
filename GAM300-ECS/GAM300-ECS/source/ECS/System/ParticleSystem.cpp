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


	void ParticleSystem::Update(Coordinator* coordinator, float deltaTime)
	{
		// Load Default shader program
		const auto& shd_ref_handle = shdrpgms[GraphicShader::Default].GetHandle();
		glUseProgram(shd_ref_handle);

		Renderer::BeginCubeBatch();

		for (auto entity : mEntities)
		{
			Particle& particle = *coordinator->GetComponent<Particle>(entity);

			Renderer::DrawCube({ 0.f, 0.f, -3.f }, { 1.f, 1.f, 1.f }, 0.f, particle.texobj_hdl);
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