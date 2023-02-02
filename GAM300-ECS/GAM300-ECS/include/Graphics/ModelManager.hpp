/******************************************************************************/
/*!
\file       ModelManager.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       16 January 2023
\brief
  This file contains the declaration of Model class.

  It also has a function which loads .obj file given the model name.

  Deprecated code for now. Not in use and there are probably minor issues with it.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

// GLM
#include <glm/glm.hpp>

// Standard Includes
#include <string>
#include <vector>

namespace Engine
{
	// Model class to store vertices and indices of loaded models
	struct Model
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
	};

	// Function is to load .obj file given the model name
	Model LoadModelFile(std::string name_);
}