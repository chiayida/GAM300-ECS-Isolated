/******************************************************************************/
/*!
\file       ParticleSystem.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       16 January 2023
\brief
  This file contains the implementation of ParticleSystem class.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#include "include/ECS/System/ParticleSystem.hpp"
#include "include/Graphics/Shader.hpp"
#include "include/Graphics/ModelManager.hpp"
#include "include/Logging.hpp"

namespace Engine
{
	void ParticleSystem::Init()
	{
		SetupBuffers();
		ShaderSetup();

		// Load Default shader program
		const auto& shd_ref_handle = shdrpgms[GraphicShader::Default].GetHandle();
		glUseProgram(shd_ref_handle);

		/*
		// Uniform for game object texture shader
		auto loc = glGetUniformLocation(shd_ref_handle, "uTextures");
		int samplers[32]{};
		for (int i = 0; i < 32; i++)
		{
			samplers[i] = i;
		}
		glUniform1iv(loc, 32, samplers);
		
		// Unload shader program
		*/
		glUseProgram(0);

		// Initialise meshes
		//GraphicImplementation::Renderer::Init();

		LOG_INSTANCE("Graphic System created");
	}


	void ParticleSystem::Update()
	{
		// Do nothing
	}


	void ParticleSystem::Update(float deltaTime)
	{
		const auto& shd_ref_handle = shdrpgms[GraphicShader::Default].GetHandle();
		glUseProgram(shd_ref_handle);

		// Bind VAO and VBO
		glEnableVertexArrayAttrib(m_ModelDataList["Cube"].va, 0);
		glBindVertexArray(m_ModelDataList["Cube"].va);
		glBindBuffer(GL_ARRAY_BUFFER, m_ModelDataList["Cube"].vb);

		// Pass VAO as position attribute to slot 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0);

		glDrawElements(GL_LINES, m_ModelDataList["Cube"].indices.size(), GL_UNSIGNED_INT, m_ModelDataList["Cube"].indices.data());

		glDisableVertexAttribArray(0);
	}


	void ParticleSystem::Destroy()
	{
		// Do nothing
	}


	void ParticleSystem::BindBuffer(GLuint& vao_, GLuint& vbo_, std::vector<float>& vertices_)
	{
		// VAO
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		// VBO
		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(GLfloat), vertices_.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void ParticleSystem::SetupBuffers()
	{
		// AABB model (cube)
		{
			Model model = LoadModelFile("cube");
			RendererData& Cube = m_ModelDataList["Cube"];
			Cube.vertices = model.vertices;
			Cube.indices = model.indices;

			BindBuffer(Cube.va, Cube.vb, Cube.vertices);
		}
	}
}