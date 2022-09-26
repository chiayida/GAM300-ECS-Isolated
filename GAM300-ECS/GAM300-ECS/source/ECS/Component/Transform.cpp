/*!*****************************************************************************
\file   Transform.cpp
\author Cruz Rolly Matthew Capiral
\par    DP email: cruzrolly.m\@digipen.edu
\date   12/9/2022

\brief
This file includes the implementation of the transform component.

Copyright (C) 2022 DigiPen Institure of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#include "include/ECS/Component/Transform.hpp"

namespace Engine
{
	glm::vec3 Transform::GetPos() const noexcept // Returns position of the object
	{
		return position;
	}

	void Transform::SetPos(glm::vec3  newPos)
	{
		position = newPos;
	}
}
