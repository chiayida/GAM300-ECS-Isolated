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
	
	{
		Transform transform = {
			{ 5.f, 5.f, 5.f },		// position
			{ 5.f, 5.f, 5.f },		// scale
			{ 5.f, 5.f, 5.f, 5.f }  // rot_q
		};
		Transform transform_ = {
			{ 10.f, 10.f, 10.f },		// position
			{ 10.f, 10.f, 10.f },		// scale
			{ 10.f, 10.f, 10.f, 10.f }  // rot_q
		};
		transform.isOverridePosition = false;
		transform.isOverrideRotation = true;
		transform.isOverrideScale = false;

		Script script1 = { "newScript2.c" };

		std::vector<Transform> vTransform{};
		vTransform.push_back(transform);
		vTransform.push_back(transform_);

		EntityID id_0 = gCoordinator.CreateEntity(); // 0
		Entity& entity = *gCoordinator.GetEntity(id_0);
		entity.SetEntityName("yesfab");
		entity.SetPrefab("transform.prefab");

		gCoordinator.AddComponent<Transform>(id_0, transform);
		gCoordinator.AddComponent<Script>(id_0, script1);
		gCoordinator.AddComponent<std::vector<Transform>>(id_0, vTransform);

		Serializer::SerializePrefab(&gCoordinator, id_0, "transform.prefab");
	}
	
	{
		Transform transform = {
			{ 1.f, 2.f, 3.f },		// position
			{ 8.f, 9.f, 10.f },		// scale
			{ 4.f, 5.f, 6.f, 7.f }  // rot_q
		};
		transform.isOverridePosition = false;
		transform.isOverrideRotation = false;
		transform.isOverrideScale = false;

		Script script2 = { "newScript3.c" };

		EntityID id_1 = gCoordinator.CreateEntity(); // 1
		Entity& entity = *gCoordinator.GetEntity(id_1);
		entity.SetEntityName("nofab");
		entity.SetPrefab("transform.prefab");

		gCoordinator.AddComponent<Transform>(id_1, transform);
		gCoordinator.AddComponent<Script>(id_1, script2);
	}

	for (auto& entity : gCoordinator.GetEntities())
	{
		if (entity.GetEntityID() == 1) continue;

		Transform* transform = gCoordinator.GetComponent<Transform>(entity);
		Transform t1 = *transform;


		std::cout << "\aaaaaaa\n";
		std::cout << "position: " << transform->position.x << " " << transform->position.y << " " << transform->position.z <<
			"\n" << "rot_q: " << transform->rot_q.x << " " << transform->rot_q.y << " " << transform->rot_q.z << " " << transform->rot_q.w <<
			"\n" << "scale: " << transform->scale.x << " " << transform->scale.y << " " << transform->scale.z <<
			"\n";

		std::cout << "\bbbbbbb\n";
		std::cout << "position: " << t1.position.x << " " << t1.position.y << " " << t1.position.z <<
			"\n" << "rot_q: " << t1.rot_q.x << " " << t1.rot_q.y << " " << t1.rot_q.z << " " << t1.rot_q.w <<
			"\n" << "scale: " << t1.scale.x << " " << t1.scale.y << " " << t1.scale.z <<
			"\n";
	}










	//Serializer::SerializeEntities(&gCoordinator, "test.scene");
	//Serializer::CreateEntityPrefab(&gCoordinator, "transform.prefab");
	//Serializer::DeserializeJson(&gCoordinator, "test.scene");

	/*
	// Printing to check
	for (auto& entity : gCoordinator.GetEntities())
	{
		std::cout << "-------------------------\nEntity\n-------------------------\n";
		std::cout << "name: " << entity.GetEntityName() << 
			"\n" << "prefab: " << entity.GetPrefab() << 
			"\n";

		if (gCoordinator.HasComponent<Transform>(entity))
		{
			Transform* transform = gCoordinator.GetComponent<Transform>(entity);
			std::cout << "\nTransform\n";
			std::cout << "position: " << transform->position.x << " " << transform->position.y << " " << transform->position.z <<
				"\n" << "rot_q: " << transform->rot_q.x << " " << transform->rot_q.y << " " << transform->rot_q.z << " " << transform->rot_q.w <<
				"\n" << "scale: " << transform->scale.x << " " << transform->scale.y << " " << transform->scale.z <<
				"\n";
		}
		
		if (gCoordinator.HasComponent<Script>(entity))
		{
			Script* script = gCoordinator.GetComponent<Script>(entity);
			std::cout << "\nScript\n";
			std::cout << "mono_string: " << script->mono_string <<
				"\n\n\n";
		}

		if (gCoordinator.HasComponent<std::vector<Transform>>(entity))
		{
			std::vector<Transform>* vtransform = gCoordinator.GetComponent<std::vector<Transform>>(entity);

			int i = 0;
			for (auto& transform : *vtransform)
			{
				std::cout << "\nTransform: " << i++ << "\n";
				std::cout << "position: " << transform.position.x << " " << transform.position.y << " " << transform.position.z <<
					"\n" << "rot_q: " << transform.rot_q.x << " " << transform.rot_q.y << " " << transform.rot_q.z << " " << transform.rot_q.w <<
					"\n" << "scale: " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z <<
					"\n";
			}
		}
	}
	*/

	return 0;
}

//FreeListAllocator_Test();
//CustomAllocator_Test();

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