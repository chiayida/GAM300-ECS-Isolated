/******************************************************************************/
/*!
\file       TagManager.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       22 December 2022
\brief
  This file contains the implementation of TagManager class.

  The Tag Manager is in charge of both tags and layers of the engine.

  It keeps track of both tags and layers and allocates them to coordinator
  entities accordingly.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#include "include/Tag/TagManager.hpp"
#include "include/Logging.hpp"
#include "include/ECS/Architecture/Coordinator.hpp"

namespace Engine
{
	TagManager::~TagManager()
	{
		mLayers.clear();
		mTags.clear();

		for (auto set : mTagsLayers)
		{
			set.second.clear();
		}
		mTagsLayers.clear();
	}


	void TagManager::InitLayers(std::set<std::string>& layers)
	{
		for (auto layer : layers)
		{
			CreateLayer(layer);
		}
	}


	void TagManager::InitTags(std::map<std::string, std::set<std::string>>& tags_layers)
	{
		// Initialise mTagsLayers and mTags
		for (auto [tag, layers] : tags_layers)
		{
			CreateTag(tag);

			for (auto layer : layers)
			{
				AddTagLayer(nullptr, tag, layer);
			}
		}
	}


	std::map<std::string, Layer> TagManager::GetLayers()
	{
		return mLayers;
	}


	std::map<std::string, Tag> TagManager::GetTags()
	{
		return mTags;
	}


	std::map<std::string, std::set<std::string>> TagManager::GetTagsLayers()
	{
		return mTagsLayers;
	}




	void TagManager::CreateTag(std::string key)
	{
		if (mTags.find(key) != mTags.end())
		{
			LOG_WARNING("Creating same tag more than once.");
			return;
		}

		// Create and insert tag to container
		mTags.insert({ key, Tag{} });
		mTagsLayers[key];
	}


	void TagManager::DeleteTag(Coordinator* coordinator, std::string key)
	{
		if (mTags.find(key) == mTags.end())
		{
			LOG_WARNING("Deleting non-existent tag.");
			return;
		}

		// Deleting tag from container
		mTags.erase(key);

		for (auto& entity : coordinator->GetEntities())
		{
			if (key == entity.GetKeyTag())
			{
				entity.SetKeyTag(std::string{});
				coordinator->SetTag(entity, Tag{});
			}
		}
		mTagsLayers.erase(key);
	}


	void TagManager::AddTagLayer(Coordinator* coordinator, std::string key, std::string layer)
	{
		if (mTags.find(key) == mTags.end() || mLayers.find(layer) == mLayers.end())
		{
			LOG_WARNING("Adding of non-existent Tag or Layer.");
			return;
		}

		mTagsLayers[key].insert(layer);

		Layer layer_ = mLayers[layer];
		Tag& tag = mTags[key];
		tag.set(layer_, true);

		if (coordinator == nullptr)
		{
			return;
		}

		Tag updated_tag = tag;
		for (auto entity : coordinator->GetEntities())
		{
			if (key == entity.GetKeyTag())
			{
				coordinator->SetTag(entity, updated_tag);
			}
		}
	}


	void TagManager::RemoveTagLayer(Coordinator* coordinator, std::string key, std::string layer)
	{
		if (mTags.find(key) == mTags.end() || mLayers.find(layer) == mLayers.end())
		{
			LOG_WARNING("Removing of non-existent Tag or Layer.");
			return;
		}

		mTagsLayers[key].erase(layer);

		Layer layer_ = mLayers[layer];
		Tag& tag = mTags[key];
		tag.set(layer_, false);

		Tag updated_tag = tag;
		for (auto entity : coordinator->GetEntities())
		{
			if (key == entity.GetKeyTag())
			{
				coordinator->SetTag(entity, updated_tag);
			}
		}
	}


	void TagManager::CreateLayer(std::string name)
	{
		if (mLayers.find(name) != mLayers.end())
		{
			LOG_WARNING("Adding layer more than once.");
			return;
		}

		// Insert layer to container
		mLayers.insert({ name, NextLayer });
		++NextLayer;
	}


	void TagManager::DeleteLayer(Coordinator* coordinator, std::string name)
	{
		if (mLayers.find(name) == mLayers.end())
		{
			LOG_WARNING("Deleting non-existent layer.");
			return;
		}

		// Remove layer and update to all containers
		// Reapply to all entities
		for (auto tag : mTags)
		{
			RemoveTagLayer(coordinator, tag.first, name);
		}
		Layer layer = mLayers[name];
		mLayers.erase(name);
	}


	void TagManager::RenameLayer(std::string oldName, std::string newName)
	{
		if (mLayers.find(oldName) == mLayers.end())
		{
			LOG_WARNING("Renaming non-existent layer.");
			return;
		}

		// Get layer from oldName and replace with newName
		Layer layer = mLayers[oldName];
		mLayers.erase(oldName);
		mLayers[newName] = layer;
	}


	void TagManager::RefreshTags(Coordinator* coordinator)
	{
		for (auto entity : coordinator->GetEntities())
		{
			Tag tag = mTags[entity.GetKeyTag()];
			coordinator->SetTag(entity, tag);
		}
	}


	void TagManager::SetEntityTag(Coordinator* coordinator, EntityID id, std::string key)
	{
		if (mTags.find(key) == mTags.end())
		{
			LOG_WARNING("Setting of non-existent tag to entity.");
			return;
		}

		// Update entity's tag and key
		Entity& entity = *coordinator->GetEntity(id);
		entity.SetKeyTag(key);

		Tag tag = mTags[key];
		coordinator->SetTag(id, tag);
	}


	void TagManager::RemoveEntityTag(Coordinator* coordinator, EntityID id, std::string key)
	{
		if (mTags.find(key) == mTags.end())
		{
			LOG_WARNING("Removing of non-existent tag to entity.");
			return;
		}

		// Update entity's tag and key
		Entity& entity = *coordinator->GetEntity(id);
		entity.SetKeyTag(std::string{});
		coordinator->SetTag(id, Tag{});
	}

} // end of namespace