/* Start Header**********************************************************************************/
/*
@file    Shader.cpp
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