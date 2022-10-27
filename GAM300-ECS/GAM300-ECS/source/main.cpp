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

	EntityID id_0 = gCoordinator.CreateEntity(); // 0
	EntityID id_1 = gCoordinator.CreateEntity(); // 1
	gCoordinator.CreateChild(id_1); // 2
	gCoordinator.CreateChild(id_1); // 3
	gCoordinator.CreateChild(2); // 4
	gCoordinator.CreateChild(4); // 5

	gCoordinator.ToChild(0, 5);

	Entity& entity_1 = *gCoordinator.GetEntity(id_1);
	gCoordinator.DuplicateEntity(entity_1, entity_1.GetParent());

	Entity& ee = *gCoordinator.GetEntity(7);
	gCoordinator.DuplicateEntity(ee, ee.GetParent());


	std::cout << "!!!Before Destroy!!!\n";
	for (auto& entity : gCoordinator.GetEntities())
	{
		std::cout << "\n";

		std::cout << "Entity ID: " << entity.GetEntityID() << "\n";

		// Parent
		if (entity.isParent())
		{
			for (auto& child : gCoordinator.GetChildObjects(entity.GetEntityID()))
			{
				std::cout << "Child ID: " << child << "\n";
			}
		}
		if (entity.IsChild())
		{
			std::cout << "Parent ID: " << entity.GetParent() << "\n";
		}

		std::cout << "\n";
	}

	gCoordinator.DestroyEntity(6);


	std::cout << "===After Destroy===\n";
	for (auto& entity : gCoordinator.GetEntities())
	{
		std::cout << "\n";

		std::cout << "Entity ID: " << entity.GetEntityID() << "\n";

		// Parent
		if (entity.isParent())
		{
			for (auto& child : gCoordinator.GetChildObjects(entity.GetEntityID()))
			{
				std::cout << "Child ID: " << child << "\n";
			}
		}
		if (entity.IsChild())
		{
			std::cout << "Parent ID: " << entity.GetParent() << "\n";
		}

		std::cout << "\n";
	}


	// Printing to check
	std::cout << "\nSTART: Printing to check\n";
	for (auto& entity : gCoordinator.GetEntities())
	{
		std::cout << "-------------------------\n"
			<< "Entity name: " << entity.GetEntityName() << "\nID: " << entity.GetEntityID();
		std::cout << "\nprefab: " << entity.GetPrefab() << "\n";

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