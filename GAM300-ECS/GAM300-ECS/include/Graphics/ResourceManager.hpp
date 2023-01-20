/* Start Header**********************************************************************************/
/*
@file    ResourceManager.hpp
@author  Chia Yi Da		c.yida@digipen.edu	100%
@date    02/07/2021
\brief
This file has the function declaration for ResourceManager


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

#include <unordered_map>

namespace Engine
{
	struct Particle;

	struct TextureContainer
	{
		GLuint texture_handle{};
		int width{}, height{};
	};

	class ResourceManager
	{
	public:
		void Create();
		void Destroy();

		void RefreshTexture(Particle* pc);

		GLuint LoadTexture(std::string filepath);
		GLuint LoadTexture(std::string filepath, int* x, int* y, int* channels_in_files, int desired_channel);

		TextureContainer GetTextureContainer(std::string name) { return textureList["Assets/" + name]; }

	private:
		GLuint LoadTextureInternal(std::string filename, int* x, int* y, int* channels_in_files, int desired_channel);
		GLuint LoadTextureInternal(std::string filename);

		std::unordered_map<std::string, TextureContainer> textureList;
	};
}