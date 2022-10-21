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
	
	/*
	{
		Transform transform = {
			{ 0.f, 0.f, 0.f },		// position
			{ 0.f, 0.f, 0.f },		// scale
			{ 0.f, 0.f, 0.f, 0.f }  // rot_q
		};
		transform.isOverridePosition = true;
		transform.isOverrideRotation = true;
		transform.isOverrideScale = true;

		Script script = { "0.c" };

		EntityID id = gCoordinator.CreateEntity(); // 0
		Entity& entity = *gCoordinator.GetEntity(id);
		entity.SetEntityName("0");
		entity.SetPrefab("transform.prefab");

		gCoordinator.AddComponent<Transform>(id, transform);
		gCoordinator.AddComponent<Script>(id, script);
	}
	
	{
		Transform transform = {
			{ 1.f, 1.f, 1.f },		// position
			{ 1.f, 1.f, 1.f },		// scale
			{ 1.f, 1.f, 1.f, 1.f }  // rot_q
		};
		transform.isOverridePosition = true;
		transform.isOverrideRotation = true;
		transform.isOverrideScale = true;

		Script script = { "1.c" };

		EntityID id = gCoordinator.CreateEntity(); // 1
		Entity& entity = *gCoordinator.GetEntity(id);
		entity.SetEntityName("1");
		entity.SetPrefab("transform.prefab");

		gCoordinator.AddComponent<Transform>(id, transform);
		gCoordinator.AddComponent<Script>(id, script);
	}
	*/

	// Created two entities from prefab.
	Serializer::CreateEntityPrefab(&gCoordinator, "transform.prefab");
	Serializer::CreateEntityPrefab(&gCoordinator, "transform.prefab");

	// Create a third one... this one more specific
	{
		Transform transform = {
			{ 4.f, 9.f, 7.f },		// position
			{ 6.f, 1.f, 9.f },		// scale
			{ 2.f, 3.f, 5.f, 9.f }  // rot_q
		};
		transform.isOverridePosition = false;
		transform.isOverrideRotation = false;
		transform.isOverrideScale = true;

		Script script = { "0.c" };

		EntityID id = gCoordinator.CreateEntity(); // 2
		Entity& entity = *gCoordinator.GetEntity(id);
		entity.SetPrefab("transform.prefab");

		gCoordinator.AddComponent<Transform>(id, transform);
		gCoordinator.AddComponent<Script>(id, script);
		gCoordinator.AddComponent<std::vector<Transform>>(id, std::vector<Transform>{});
	}


	// Change entity 1 transform to all 2s
	// Update prefab to entity 1's properties
	{
		Entity& entity = *gCoordinator.GetEntity(1);

		if (gCoordinator.HasComponent<Transform>(entity))
		{
			Transform* ptr = gCoordinator.GetComponent<Transform>(entity);

			Transform transform = {
				{ 123.f, 456.f, 789.f },	// position
				{ 11.f, 22.f, 33.f },		// scale
				{ 44.f, 55.f, 66.f, 77.f }  // rot_q
			};
			transform.isOverridePosition = true;
			transform.isOverrideRotation = false;
			transform.isOverrideScale = true;

			*ptr = transform;
		}

		Serializer::SerializePrefab(&gCoordinator, 1, "transform.prefab");
	}

	Serializer::ApplyUpdatedPrefab(&gCoordinator, "transform.prefab");

	// Printing to check
	std::cout << "\nSTART: Printing to check\n";
	for (auto& entity : gCoordinator.GetEntities())
	{
		std::cout << "-------------------------\n" 
				  << "Entity name: " << entity.GetEntityName() << " ID: " << entity.GetEntityID() 
				  << "\n-------------------------\n";
		std::cout << "prefab: " << entity.GetPrefab() << "\n";

		if (gCoordinator.HasComponent<Transform>(entity))
		{
			Transform* transform = gCoordinator.GetComponent<Transform>(entity);

			std::cout << "\nTransform\n";
			std::cout << "position: " << transform->position.x << " " << transform->position.y << " " << transform->position.z <<
				"\n" << "rot_q: " << transform->rot_q.x << " " << transform->rot_q.y << " " << transform->rot_q.z << " " << transform->rot_q.w <<
				"\n" << "scale: " << transform->scale.x << " " << transform->scale.y << " " << transform->scale.z <<
				"\n" << "override: " << transform->isOverridePosition << " " << transform->isOverrideRotation << " " << transform->isOverrideScale <<
				"\n";
		}
		
		if (gCoordinator.HasComponent<Script>(entity))
		{
			Script* script = gCoordinator.GetComponent<Script>(entity);
			std::cout << "\nScript\n";
			std::cout << "mono_string: " << script->mono_string <<
				"\n";
		}

		if (gCoordinator.HasComponent<std::vector<Transform>>(entity))
		{
			std::vector<Transform>* vtransform = gCoordinator.GetComponent<std::vector<Transform>>(entity);
			
			int i = 0;
			for (auto& transform : *vtransform)
			{
				std::cout << "\nTransform index: " << i++ << "\n";
				std::cout << "position: " << transform.position.x << " " << transform.position.y << " " << transform.position.z <<
					"\n" << "rot_q: " << transform.rot_q.x << " " << transform.rot_q.y << " " << transform.rot_q.z << " " << transform.rot_q.w <<
					"\n" << "scale: " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z <<
					"\n";
			}
		}
	}
	std::cout << "\nEND: Printing to check\n";

	return 0;
}

//FreeListAllocator_Test();
//CustomAllocator_Test();

//Serializer::SerializeEntities(&gCoordinator, "test.scene");
//Serializer::CreateEntityPrefab(&gCoordinator, "transform.prefab");
//Serializer::DeserializeJson(&gCoordinator, "test.scene");

/*
enum class color
{
	red,
	green,
	blue
};

struct point2d
{
	point2d() {}
	point2d(int x_, int y_) : x(x_), y(y_) {}
	int x = 0;
	int y = 0;
};

struct shape
{
	shape(std::string n) : name(n) {}
	shape() = default;
	void set_visible(bool v) { visible = v; }
	bool get_visible() const { return visible; }

	color color_ = color::blue;
	std::string name = "";
	point2d position;
	std::map<color, point2d> dictionary;

	RTTR_ENABLE()
private:
	bool visible = false;
};

struct circle : shape
{
	circle(std::string n) : shape(n) {}
	circle() = default;
	double radius = 5.2;
	std::vector<point2d> points;

	int no_serialize = 100;

	RTTR_ENABLE(shape)
};

struct Container
{
	std::unordered_map<int, circle> map;
	std::vector<int> array = { 10,1 };
	RTTR_ENABLE()
};


RTTR_REGISTRATION
{
rttr::registration::class_<shape>("shape")
.property("visible", &shape::get_visible, &shape::set_visible)
.property("color", &shape::color_)
.property("name", &shape::name)
.property("position", &shape::position)
.property("dictionary", &shape::dictionary)
;

rttr::registration::class_<circle>("circle")
.property("radius", &circle::radius)
.property("points", &circle::points)
.property("no_serialize", &circle::no_serialize)
(
metadata("NO_SERIALIZE", true)
)
;

rttr::registration::class_<point2d>("point2d")
.property("x", &point2d::x)
.property("y", &point2d::y)
;


rttr::registration::enumeration<color>("color")
(
value("red", color::red),
value("blue", color::blue),
value("green", color::green)
);




rttr::registration::class_<Container>("circle")
.property("Hash Map", &Container::map)
.property("Array", &Container::array)
;
}
*/


/*
std::string json_string;
Container map;
{
	circle c_1("Circle #1");
	shape& my_shape = c_1;

	c_1.set_visible(true);
	c_1.points = std::vector<point2d>(2, point2d(1, 1));
	c_1.points[1].x = 23;
	c_1.points[1].y = 42;

	c_1.position.x = 12;
	c_1.position.y = 66;

	c_1.radius = 5.123;
	c_1.color_ = color::red;

	// additional braces are needed for a VS 2013 bug
	c_1.dictionary = { { {color::green, {1, 2} }, {color::blue, {3, 4} }, {color::red, {5, 6} } } };

	c_1.no_serialize = 12345;

	json_string = Serializer::InstanceToJson(my_shape, "Circle 1").dump(4); // serialize the circle to 'json_string'
	map.map[0] = c_1;
}

std::cout << json_string << std::endl;

circle c_2("Circle #2"); // create a new empty circle
map.map[1] = c_2;


// from_json(json_string, c_2); // deserialize it with the content of 'c_1'
std::cout << "\n############################################\n" << std::endl;

std::cout << Serializer::InstanceToJson(c_2, "Circle 2").dump(4) << std::endl;

json_string = Serializer::InstanceToJson(map, "Map").dump(4);
std::cout << "Map:\n " << std::endl;
std::cout << json_string << std::endl;
return 0;
*/