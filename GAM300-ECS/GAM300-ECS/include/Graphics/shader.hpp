/******************************************************************************/
/*!
\file       Shader.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       16 January 2023
\brief
  This file contains the container for different shaders and 
  a function which is to be called to set up the shaders.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "GLSLShader.hpp"
#include <map>

namespace Engine
{
	enum class GraphicShader
	{
		Default = 0, // Game objects
		Collision,
		Font_Draw,
		Simple_Depth,
		Light
	};

    void ShaderSetup();
	
	extern std::map<GraphicShader, GLSLShader> shdrpgms;
}