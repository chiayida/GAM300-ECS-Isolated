// To check for memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include "include/Memory/MemoryUnitTest.hpp"
#include "include/ECS/Architecture/Coordinator.hpp"
#include "include/Serialization/Serializer.hpp"
#include "include/Tag/TagManager.hpp"

#include "include/ECS/Component/Transform.hpp"
#include "include/ECS/System/TransformSystem.hpp"

// #define MEMORY_SIZE 1e8 // 1 * 10^8 bytes	

using namespace Engine;

int main()
{
	// Memory leak check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	TagManager gTagManager;
	Serializer::DeserializeLayers(&gTagManager, "appLayers.layer");
	Serializer::DeserializeTags(&gTagManager, "appTags.tag");

	Coordinator gCoordinator;
	gCoordinator.Init();

	EntityID entity_0 = gCoordinator.CreateEntity(); // 0
	EntityID entity_1 = gCoordinator.CreateEntity(); // 1
	EntityID entity_2 = gCoordinator.CreateEntity(); // 2
	EntityID entity_3 = gCoordinator.CreateEntity(); // 3
	EntityID entity_4 = gCoordinator.CreateEntity(); // 4
	EntityID entity_5 = gCoordinator.CreateEntity(); // 5
	EntityID entity_6 = gCoordinator.CreateEntity(); // 6
	
	// In Init first, inspector when prompt
	//////////////////////////////////////////

	/*
	gTagManager.CreateTag("Default");
	gTagManager.CreateTag("Ignore Raycast");
	gTagManager.CreateTag("UI");
	gTagManager.CreateTag("Ground");
	gTagManager.CreateTag("Interactables");
	gTagManager.CreateTag("Player");
	gTagManager.CreateTag("Spells");
			   
	gTagManager.CreateLayer("Spells");
	gTagManager.CreateLayer("Player");
	gTagManager.CreateLayer("Interactables");
	gTagManager.CreateLayer("Ground");
	gTagManager.CreateLayer("UI");
	gTagManager.CreateLayer("Ignore Raycast");
	gTagManager.CreateLayer("Default");
	*/

	//////////////////////////////////////////
	//////////////////////////////////////////

	/*
	gTagManager.AddTagLayer(&gCoordinator, "Default", "Spells");
	gTagManager.AddTagLayer(&gCoordinator, "Default", "Player");
	gTagManager.AddTagLayer(&gCoordinator, "Default", "Interactables");
	gTagManager.AddTagLayer(&gCoordinator, "Default", "Ground");
	gTagManager.AddTagLayer(&gCoordinator, "Default", "UI");
	gTagManager.AddTagLayer(&gCoordinator, "Default", "Ignore Raycast");
	gTagManager.AddTagLayer(&gCoordinator, "Default", "Default");

	gTagManager.AddTagLayer(&gCoordinator, "Ignore Raycast", "Spells");
	gTagManager.AddTagLayer(&gCoordinator, "Ignore Raycast", "Player");
	gTagManager.AddTagLayer(&gCoordinator, "Ignore Raycast", "Interactables");
	gTagManager.AddTagLayer(&gCoordinator, "Ignore Raycast", "Ground");
	gTagManager.AddTagLayer(&gCoordinator, "Ignore Raycast", "UI");
	gTagManager.AddTagLayer(&gCoordinator, "Ignore Raycast", "Ignore Raycast");

	gTagManager.AddTagLayer(&gCoordinator, "UI", "Spells");
	gTagManager.AddTagLayer(&gCoordinator, "UI", "Player");
	gTagManager.AddTagLayer(&gCoordinator, "UI", "Interactables");
	gTagManager.AddTagLayer(&gCoordinator, "UI", "Ground");
	gTagManager.AddTagLayer(&gCoordinator, "UI", "UI");

	gTagManager.AddTagLayer(&gCoordinator, "Ground", "Spells");
	gTagManager.AddTagLayer(&gCoordinator, "Ground", "Player");
	gTagManager.AddTagLayer(&gCoordinator, "Ground", "Interactables");
	gTagManager.AddTagLayer(&gCoordinator, "Ground", "Ground");

	gTagManager.AddTagLayer(&gCoordinator, "Interactables", "Player");
	gTagManager.AddTagLayer(&gCoordinator, "Interactables", "Interactables");

	gTagManager.AddTagLayer(&gCoordinator, "Player", "Player");

	gTagManager.AddTagLayer(&gCoordinator, "Spells", "Spells");
	*/

	//////////////////////////////////////////
	//////////////////////////////////////////

	/*
	gTagManager.SetEntityTag(&gCoordinator, entity_0, "Default");
	gTagManager.SetEntityTag(&gCoordinator, entity_1, "Ignore Raycast");
	gTagManager.SetEntityTag(&gCoordinator, entity_2, "UI");
	gTagManager.SetEntityTag(&gCoordinator, entity_3, "Ground");
	gTagManager.SetEntityTag(&gCoordinator, entity_4, "Interactables");
	gTagManager.SetEntityTag(&gCoordinator, entity_5, "Player");
	gTagManager.SetEntityTag(&gCoordinator, entity_6, "Spells");
	*/

	gTagManager.SetEntityTag(&gCoordinator, entity_0, "Default");
	gTagManager.SetEntityTag(&gCoordinator, entity_1, "Default");
	gTagManager.SetEntityTag(&gCoordinator, entity_2, "Default");
	gTagManager.SetEntityTag(&gCoordinator, entity_3, "Ground");
	gTagManager.SetEntityTag(&gCoordinator, entity_4, "Ground");
	gTagManager.SetEntityTag(&gCoordinator, entity_5, "Ground");
	gTagManager.SetEntityTag(&gCoordinator, entity_6, "Ground");

	std::cout << "\n";
	//////////////////////////////////////////
	//////////////////////////////////////////

	for (auto layer : gTagManager.GetLayers())
	{
		std::cout << layer.first << ": " << layer.second << "\n";
	}
	std::cout << "=====\n";
	for (auto tag : gTagManager.GetTags())
	{
		std::cout << tag.first << ": " << tag.second << "\n";
	}
	std::cout << "=====\n";

	for (auto entity : gCoordinator.GetEntities())
	{
		EntityID id = entity.GetEntityID();
		std::cout << id << ": " << gCoordinator.GetTag(entity) << " | " << entity.GetKeyTag() << "\n";
	}

	std::cout << "\nEND\n\n";


	//Serializer::SerializeLayers(&gTagManager, "appLayers.layer");
	//Serializer::SerializeTags(&gTagManager, "appTags.tag");



	/*
	std::cout << "\n==========================\n       All Entities\n==========================\n";
	for (auto& entity : gCoordinator.GetEntities())
	{
		std::cout << "id: " << entity.GetEntityID() << " " << entity.isParent() << " " << entity.IsChild() << "\n";
	}


	std::cout << "\n===========================\n     Parent-Child Tree\n===========================\n";
	for (auto& parent : gCoordinator.GetMap())
	{
		std::cout << "Parent: " << parent.first << " Children: ";

		for (auto& child : parent.second)
		{
			std::cout << child << " ";
		}
		std::cout << "\n";
	}


	std::cout << "\n===========================\n      Prefab Container\n===========================\n";
	std::cout << "\parentchild.PREFAB\n";
	std::vector<EntityID>& prefab_container1 = gCoordinator.GetPrefabContainer("parentchild.prefab");
	for (auto id : prefab_container1)
	{
		std::cout << "id: " << id << " Prefab: " << gCoordinator.GetEntity(id)->GetPrefab() << "\n";
	}

	std::cout << "\nSINGLE.PREFAB\n";

	std::vector<EntityID>& prefab_container2 = gCoordinator.GetPrefabContainer("single.prefab");
	for (auto id : prefab_container2)
	{
		std::cout << "id: " << id << " Prefab: " << gCoordinator.GetEntity(id)->GetPrefab() << "\n";
	}


	std::cout << "\n==========================\n        Components\n==========================\n";
	for (auto& entity : gCoordinator.GetEntities())
	{
		std::cout << "-------------------------\n"
			<< "Entity name: " << entity.GetEntityName() << "\nEntity ID: " << entity.GetEntityID() << " Parent ID: " << entity.GetParent();
		std::cout << "\nprefab: " << entity.GetPrefab() << "\n";

		if (gCoordinator.HasComponent<Transform>(entity))
		{
			Transform* transform = gCoordinator.GetComponent<Transform>(entity);

			std::cout << "\nTransform:\n";
			std::cout << "position: " << transform->position.x << " " << transform->position.y << " " << transform->position.z <<
				"\n" << "scale: " << transform->scale.x << " " << transform->scale.y << " " << transform->scale.z <<
				"\n" << "rot_q: " << transform->rot_q.w << " " << transform->rot_q.x << " " << transform->rot_q.y << " " << transform->rot_q.z <<
				"\n" << "isOverridePosition: " << transform->isOverridePosition << " isOverrideScale: " << transform->isOverrideScale << " isOverrideRotation: " << transform->isOverrideRotation <<
				"\n";
		}

		if (gCoordinator.HasComponent<Script>(entity))
		{
			Script* script = gCoordinator.GetComponent<Script>(entity);
			std::cout << "\nScript:\n";
			std::cout << "mono_string: " << script->mono_string <<
				"\n";
		}

		if (gCoordinator.HasComponent<std::vector<Transform>>(entity))
		{
			std::cout << "\nstd::vector<Transform>:";
			std::vector<Transform>* vtransform = gCoordinator.GetComponent<std::vector<Transform>>(entity);

			int i = 0;
			for (auto& transform : *vtransform)
			{
				std::cout << "\nTransform index: " << i++ << "\n";
				std::cout << "position: " << transform.position.x << " " << transform.position.y << " " << transform.position.z <<
					"\n" << "scale: " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z <<
					"\n" << "rot_q: " << transform.rot_q.w << " " << transform.rot_q.x << " " << transform.rot_q.y << " " << transform.rot_q.z <<
					"\n";
			}
		}
	}
	std::cout << "\n\n\n";
	*/

	return 0;
}

//FreeListAllocator_Test();
//CustomAllocator_Test();

//Serializer::SerializeEntities(&gCoordinator, "test.scene");
//Serializer::DeserializeJson(&gCoordinator, "test.scene");


/*
// Initialise components for individual entities
for (auto& entity : gCoordinator.GetEntities())
{
	Script script;
	Transform transform;
	Transform transform_;

	EntityID entity_id = entity.GetEntityID();
	float id = static_cast<float>(entity_id);

	transform.position = { id, id + 1, id + 2 };
	transform.scale = { id, id, id };
	transform.rot_q = { id, id + 1, id + 2, id + 3 };

	transform_.position = { id, id - 1, id - 2 };
	transform_.scale = { id, id, id };
	transform_.rot_q = { id, id - 1, id - 2, id - 3 };

	script.mono_string = std::to_string(entity_id) + ".cs";
	entity.SetPrefab(std::to_string(entity_id) + ".prefab");

	gCoordinator.AddComponent<Transform>(entity, transform);
	gCoordinator.AddComponent<Script>(entity, script);
	gCoordinator.AddComponent<std::vector<Transform>>(entity, std::vector<Transform>{ transform, transform_ });
}
*/