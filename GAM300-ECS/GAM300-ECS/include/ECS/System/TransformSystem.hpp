/******************************************************************************/
/*!
\file       TransformSystem.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       18 August 2022
\brief
  This file contains the declaration of TransformSystem class.

  It is just for me to play around and test whatever I am implementing at the
  moment.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
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