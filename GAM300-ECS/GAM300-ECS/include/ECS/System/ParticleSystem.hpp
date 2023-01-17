/******************************************************************************/
/*!
\file       ParticleSystem.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       16 January 2023
\brief
  This file contains the declaration of ParticleSystem class.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "include/ECS/Architecture/System.hpp"
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

namespace Engine
{
	class ParticleSystem : public System
	{
	public:
		ParticleSystem() = default;
		void Init() override;
		void Update() override;
		void Update(float deltaTime) override;
		void Destroy() override;
	};
}