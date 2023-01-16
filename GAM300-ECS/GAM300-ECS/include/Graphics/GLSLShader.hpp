/* Start Header**********************************************************************************/
/*
@file    GLSLShader.hpp
@author  Chia Yi Da		c.yida@digipen.edu
@date    23/06/2021
\brief
This file contains declaration of member functions of class GLSLShader.


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **********************************************************************************/
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

namespace Engine
{
    class GLSLShader
    {
    public:
        enum class ShaderType
        {
            Vertex_Shader = GL_VERTEX_SHADER,
            Fragment_Shader = GL_FRAGMENT_SHADER
        };

        GLuint id{};

        // default ctor required to initialize GLSLShader object to safe state
        GLSLShader() : pgm_handle(0), is_linked(GL_FALSE) {}

        GLboolean CompileLinkValidate(std::vector<std::pair<ShaderType, std::string>>);
        GLboolean CompileShaderFromFile(ShaderType shader_type, std::string const& file_name);
        GLboolean CompileShaderFromString(ShaderType shader_type, std::string const& shader_src);

        // Link shader objects attached to handle pgm_handle. This member function
        // will also verify the status of the link operation (successful or not?).
        // If the shader objects did not link into a program object, then the
        // member function must retrieve and write the program object's information
        // log to data member log_string. 
        GLboolean Link();

        // Install the shader program object whose handle is encapsulated
        // by member pgm_handle
        void Use();

        // De-install previously installed shader program object using Use().
        // More correctly, after the execution of this member function, the
        // current rendering state is referring to an invalid program object.
        void UnUse();

        // check whether the executable shader program object can execute given the
        // current OpenGL state ...
        // See the glValidateProgram() reference page for more information
        // The function returns true if validatation succeeded 
        GLboolean Validate();

        GLuint GetHandle() const;
        GLboolean IsLinked() const;
        std::string GetLog() const;

        void DeleteShaderProgram();

        // Uniform Overloading for possibilities
        static void SetUniform(GLchar const* name, GLboolean val, GLuint prgm_handle);
        static void SetUniform(GLchar const* name, GLint val, GLuint prgm_handle);
        static void SetUniform(GLchar const* name, GLfloat val, GLuint prgm_handle);
        static void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLuint prgm_handle);
        static void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLuint prgm_handle);
        static void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLuint prgm_handle);
        static void SetUniform(GLchar const* name, glm::vec2 const& val, GLuint prgm_handle);
        static void SetUniform(GLchar const* name, glm::vec3 const& val, GLuint prgm_handle);
        static void SetUniform(GLchar const* name, glm::vec4 const& val, GLuint prgm_handle);
        static void SetUniform(GLchar const* name, glm::mat3 const& val, GLuint prgm_handle);
        static void SetUniform(GLchar const* name, glm::mat4 const& val, GLuint prgm_handle);

    private:
        GLuint pgm_handle = 0;
        GLboolean is_linked = GL_FALSE;
        std::string log_string;

        GLboolean FileExists(std::string const& file_name);
    };
}