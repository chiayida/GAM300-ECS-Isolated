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
	class Particle : public IComponent
	{
	public:
		Particle() : minUV{ 0.f, 0.f }, maxUV{ 1.f, 1.f } {}

		// Texture properties
		std::string filepath;
		std::string textureName;

		GLuint texobj_hdl{};
		GLint width{}, height{}, BPP{};

		glm::vec2 minUV, maxUV;

		RTTR_ENABLE(IComponent);
	};
}