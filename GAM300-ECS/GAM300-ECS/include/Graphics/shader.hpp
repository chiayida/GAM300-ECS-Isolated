/* Start Header**********************************************************************************/
/*
@file    Shader.hpp
@author  Chia Yi Da		c.yida@digipen.edu
@date    23/06/2021
\brief
This file contains definitions of member functions of class GLSLShader.


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **********************************************************************************/
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
	
	static std::map<GraphicShader, GLSLShader> shdrpgms;
}