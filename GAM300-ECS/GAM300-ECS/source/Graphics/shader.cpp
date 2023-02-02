/******************************************************************************/
/*!
\file       Shader.cpp
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
#include "include/Graphics/Shader.hpp"

namespace Engine
{
    std::map<GraphicShader, GLSLShader> shdrpgms;

    // Function that sets up all shader files
    void ShaderSetup()
    {
        // Default Shader for Game objects
        std::vector<std::pair<GLSLShader::ShaderType, std::string>> shdr_files
        {
            std::make_pair(GLSLShader::ShaderType::Vertex_Shader, "Assets/Default.vert"),
            std::make_pair(GLSLShader::ShaderType::Fragment_Shader, "Assets/Default.frag")
        };

        GLSLShader shdr_pgm;
        shdr_pgm.CompileLinkValidate(shdr_files);

        if (GL_FALSE == shdr_pgm.IsLinked())
        {
            std::cout << "Unable to compile/link/validate shader programs\n";
            std::cout << shdr_pgm.GetLog() << "\n";
            std::exit(EXIT_FAILURE);
        }

        shdrpgms.insert(std::pair<GraphicShader, GLSLShader>(GraphicShader::Default, shdr_pgm));
    }
}