/*!*****************************************************************************
\file   TransformSystem.hpp
\author Cruz Rolly Matthew Capiral
\par    DP email: cruzrolly.m\@digipen.edu
\date   12/9/2022

\brief
This file includes the definition of the transform system.

The transform system is where the transform matrix is calculated to store in each
transform component.

Copyright (C) 2022 DigiPen Institure of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#pragma once

#include "include/ECS/Architecture/System.hpp"

namespace Engine
{
	class TransformSystem : public System
	{
	public:
		TransformSystem() = default;
		void Init() override;
		void Update() override;
		void Destroy() override;
	};
}