/*!*****************************************************************************
\file   TransformSystem.cpp
\author Cruz Rolly Matthew Capiral
\par    DP email: cruzrolly.m\@digipen.edu
\date   12/9/2022

\brief
This file includes the implementation of the transform system.

The transform system is where the transform matrix is calculated to store in each
transform component.

Copyright (C) 2022 DigiPen Institure of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#include "include/ECS/System/TransformSystem.hpp"
#include "include/ECS/Component/Transform.hpp"

namespace Engine
{
	void TransformSystem::Init()
	{
		for(auto entity : mEntities)
		{
			// do nothing
		}
	}

	void TransformSystem::Update()
	{
		for (auto entity : mEntities)
		{
			// do nothing
		}
	}

	void TransformSystem::Destroy()
	{
		
	}
}
