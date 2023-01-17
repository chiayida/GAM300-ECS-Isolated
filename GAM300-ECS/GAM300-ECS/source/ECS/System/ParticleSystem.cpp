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
#include "include/Graphics/Shader.hpp"
#include "include/Logging.hpp"

namespace Engine
{
	void ParticleSystem::Init()
	{
		LOG_INSTANCE("Graphic System created");
	}


	void ParticleSystem::Update()
	{
		// Do nothing
	}


	void ParticleSystem::Update(float deltaTime)
	{
		//LOG_INSTANCE("Graphic System Updating");
	}


	void ParticleSystem::Destroy()
	{
		// Do nothing
	}
}