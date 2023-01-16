/******************************************************************************/
/*!
\file   ModelManager.hpp
\author Chia Yi Da
\par    email: c.yida\@digipen.edu
\par    DigiPen login: c.yida
\par    Course: CSD3150
\par    Assignment #1
\date   6/01/2022
\brief
  Function declaration for Model class.
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