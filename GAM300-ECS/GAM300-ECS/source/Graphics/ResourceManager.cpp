/******************************************************************************/
/*!
\file       ResourceManager.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       16 January 2023
\brief
	This file contains the implementation of ResourceManager class that
	loads the necessary 2D textures.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#include "stb_image/stb_image.h"
#include "include/Graphics/ResourceManager.hpp"
#include "include/ECS/Component/Particle.hpp"
#include "include/Logging.hpp"

namespace Engine
{
	// Create function for ResourceManager
	void ResourceManager::Create()
	{
		// OpenGL - Cartesian coordinate system
		// Flips image in vertically; PNG - top left
		stbi_set_flip_vertically_on_load(1);

		LOG_INSTANCE("Resource Manager created");
	}

	// Destroy function for ResourceManager
	void ResourceManager::Destroy()
	{
		// For textures
		for (auto& a : textureList)
		{
			glDeleteTextures(1, &a.second.texture_handle);
		}
		textureList.clear();

		LOG_INSTANCE("Resource Manager destroyed");
	}

	void ResourceManager::RefreshTexture(Particle* pc)
	{
		/*
		Particle& texture = *pc;
		stbi_uc* temBuff = stbi_load(texture.textureName.c_str(), &(texture.width), &(texture.height), &(texture.BPP), 4);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureSubImage2D(textureList[texture.textureName].texture_handle, 0, 0, 0, (texture.width), (texture.height), GL_RGBA, GL_UNSIGNED_BYTE, temBuff);
		stbi_image_free(temBuff);
		*/
	}

	// Function loads texture into container without checking if it exist
	GLuint ResourceManager::LoadTextureInternal(std::string filename, int* x, int* y, int* channels_in_files, int desired_channel)
	{
		// Read png file
		stbi_uc* temBuff = stbi_load(filename.c_str(), x, y, channels_in_files, desired_channel);

		GLuint texobj_hdl{};
		glCreateTextures(GL_TEXTURE_2D, 1, &texobj_hdl);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureStorage2D(texobj_hdl, 1, GL_RGBA8, *x, *y);
		glTextureSubImage2D(texobj_hdl, 0, 0, 0, *x, *y, GL_RGBA, GL_UNSIGNED_BYTE, temBuff);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (temBuff) stbi_image_free(temBuff);

		// Store in container to be reused later
		textureList[filename].texture_handle = texobj_hdl;
		textureList[filename].width = *x;
		textureList[filename].height = *y;

		return textureList[filename].texture_handle;
	}

	// Function loads texture into container without checking if it exist
	GLuint ResourceManager::LoadTextureInternal(std::string filename)
	{
		int width = 0, height = 0, BPP = 0;

		int* x = &width;
		int* y = &height;
		int* channels_in_files = &BPP;

		// Read png file
		stbi_uc* temBuff = stbi_load(filename.c_str(), x, y, channels_in_files, 4);

		GLuint texobj_hdl{};
		glCreateTextures(GL_TEXTURE_2D, 1, &texobj_hdl);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureStorage2D(texobj_hdl, 1, GL_RGBA8, *x, *y);
		glTextureSubImage2D(texobj_hdl, 0, 0, 0, *x, *y, GL_RGBA, GL_UNSIGNED_BYTE, temBuff);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (temBuff) stbi_image_free(temBuff);

		// Store in container to be reused later
		textureList[filename].texture_handle = texobj_hdl;
		textureList[filename].width = *x;
		textureList[filename].height = *y;

		return textureList[filename].texture_handle;
	}
	
	// Function loads texture into container if it exist
	GLuint ResourceManager::LoadTexture(std::string filepath, int* x, int* y, int* channels_in_files, int desired_channel)
	{
		// Check if it exist first before loading
		std::string filepath_ = "Assets/" + filepath;
		if (textureList.find(filepath_) != textureList.end())
		{
			*x = textureList[filepath_].width;
			*y = textureList[filepath_].height;

			return textureList[filepath_].texture_handle;
		}

		return LoadTextureInternal(filepath_, x, y, channels_in_files, desired_channel);
	}

	// Function loads texture into container if it doesnt exist
	GLuint ResourceManager::LoadTexture(std::string filepath)
	{
		// Check if it exist first before loading
		std::string filepath_ = "Assets/" + filepath;
		if (textureList.find(filepath_) != textureList.end())
		{
			return textureList[filepath_].texture_handle;
		}

		return LoadTextureInternal(filepath_);
	}
}