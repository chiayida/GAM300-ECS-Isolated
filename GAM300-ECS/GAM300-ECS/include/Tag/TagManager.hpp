/******************************************************************************/
/*!
\file       TagManager.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       22 December 2022
\brief
  This file contains the declaration of TagManager class.

  The Tag Manager is in charge of both tags and layers of the engine.

  It keeps track of both tags and layers and allocates them to coordinator
  entities accordingly.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "include/ECS/Architecture/ECS.hpp"
#include <map>
#include <set>

namespace Engine
{
	class Coordinator;

	class TagManager
	{
	public:
		~TagManager();
		void InitLayers(std::set<std::string>& layers);
		void InitTags(std::map<std::string, std::set<std::string>>& tags_layers);

		std::map<std::string, Layer> GetLayers();
		std::map<std::string, Tag> GetTags();
		std::map<std::string, std::set<std::string>> GetTagsLayers();

		// Cannot rename tag (Same as Unity)
		void CreateTag(std::string key);
		void DeleteTag(Coordinator* coordinator, std::string key);

		void CreateLayer(std::string name);
		void DeleteLayer(Coordinator* coordinator, std::string name);

		void AddTagLayer(Coordinator* coordinator, std::string key, std::string layer);
		void RemoveTagLayer(Coordinator* coordinator, std::string key, std::string layer);

		void RenameLayer(std::string oldName, std::string newName);
		void RefreshTags(Coordinator* coordinator);

		void SetEntityTag(Coordinator* coordinator, EntityID id, std::string key);
		void RemoveEntityTag(Coordinator* coordinator, EntityID id, std::string key);
	private:
		Layer NextLayer{};
		std::map<std::string, Layer> mLayers{};
		std::map<std::string, Tag> mTags{};
		std::map<std::string, std::set<std::string>> mTagsLayers{};
	};

} // end of namespace