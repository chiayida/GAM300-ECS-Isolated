// To check for memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include "include/Memory/MemoryUnitTest.hpp"
#include "include/ECS/Architecture/Coordinator.hpp"
#include "include/Serialization/Serializer.hpp"

#include "include/ECS/Component/Transform.hpp"

// #define MEMORY_SIZE 1e8 // 1 * 10^8 bytes

using namespace Engine;

int main()
{
	// Memory leak check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Coordinator gCoordinator;
	gCoordinator.Init();

	// Initialise Entities
	gCoordinator.CreateEntity(); // E0
	gCoordinator.CreateEntity(); // E1


	// Initialise components for individual entities
	for (auto& entity : gCoordinator.GetEntities())
	{
		Script script;
		Transform transform;

		EntityID entity_id = entity.GetEntityID();
		float id = static_cast<float>(entity_id);

		if (entity_id == 0)
		{
			transform.position = { 1, 1, 1 };
			transform.scale = { 2, 2, 2 };
			transform.rot_q = { 3, 3, 3, 3 };

			transform.isOverridePosition = true;
			transform.isOverrideRotation = false;
			transform.isOverrideScale = true;
		}
		else
		{
			transform.position = { 4, 4, 4 };
			transform.scale = { 5, 5, 5 };
			transform.rot_q = { 6, 6, 6, 6 };

			transform.isOverridePosition = false;
			transform.isOverrideRotation = false;
			transform.isOverrideScale = true;
		}

		script.mono_string = std::to_string(entity_id) + ".cs";
		entity.SetPrefab("transform.prefab");

		gCoordinator.AddComponent<Transform>(entity, transform);
		gCoordinator.AddComponent<Script>(entity, script);
	}

	// Save Prefab
	Serializer::SerializePrefab(&gCoordinator, 0, "transform.prefab");
	Serializer::ApplyUpdatedPrefab(&gCoordinator, "transform.prefab");


	std::cout << "\n==========================\n       All Entities\n==========================\n";
	for (auto& entity : gCoordinator.GetEntities())
	{
		std::cout << "id: " << entity.GetEntityID() << "\n";
	}


	/*
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
	*/
	

	std::cout << "\n==========================\n        Components\n==========================\n";
	for (auto& entity : gCoordinator.GetEntities())
	{
		std::cout << "-------------------------\n"
			<< "Entity name: " << entity.GetEntityName() << "\nID: " << entity.GetEntityID();
		std::cout << "\nprefab: " << entity.GetPrefab() << "\n";

		if (gCoordinator.HasComponent<Transform>(entity))
		{
			Transform* transform = gCoordinator.GetComponent<Transform>(entity);

			std::cout << "\nTransform:\n";
			std::cout << "position: " << transform->position.x << " " << transform->position.y << " " << transform->position.z <<
				"\n" << "rot_q: " << transform->rot_q.w << " " << transform->rot_q.x << " " << transform->rot_q.y << " " << transform->rot_q.z <<
				"\n" << "scale: " << transform->scale.x << " " << transform->scale.y << " " << transform->scale.z <<
				"\n" << "isOverride: " << transform->isOverridePosition << " " << transform->isOverrideScale << " " << transform->isOverrideRotation <<
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
					"\n" << "rot_q: " << transform.rot_q.w << " " << transform.rot_q.x << " " << transform.rot_q.y << " " << transform.rot_q.z <<
					"\n" << "scale: " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z <<
					"\n";
			}
		}
	}

	std::cout << "\n\n\n";

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