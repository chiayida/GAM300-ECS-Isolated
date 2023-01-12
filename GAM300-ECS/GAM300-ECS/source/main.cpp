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
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// #define MEMORY_SIZE 1e8 // 1 * 10^8 bytes	

using namespace Engine;

int main()
{
#if 0
	// Memory leak check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	TagManager gTagManager;
	Serializer::DeserializeLayers(&gTagManager, "layers.layer");
	Serializer::DeserializeTags(&gTagManager, "tags.tag");

	Coordinator gCoordinator;
	gCoordinator.Init();
	Serializer::DeserializeJson(&gCoordinator, &gTagManager, "test.scene");
	
	//////////////////////////////////////////

	std::cout << "\n";
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
#endif

	GLFWwindow *window;

	/* Initialize the library */
	if ( !glfwInit() )
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if ( !window )
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while ( !glfwWindowShouldClose(window) )
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
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