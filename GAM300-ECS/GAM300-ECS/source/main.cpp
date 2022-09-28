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
	gCoordinator.CreateEntity(); // 0
	gCoordinator.CreateEntity(); // 1
	gCoordinator.CreateEntity(); // 2

	for (auto& e : gCoordinator.GetEntities())
	{
		gCoordinator.AddComponent<Transform>(e);

		Transform* transform = gCoordinator.GetComponent<Transform>(e);
		std::map<unsigned int, std::vector<unsigned int>> map{};

		if (e.GetEntityID() == 0)
		{
			map = { { 0, {1, 2, 3, 4, 5} } };
			transform->map = map;
			transform->position = {};
		}
		else if (e.GetEntityID() == 1)
		{
			transform->map = map;
			transform->position = { 1.f, 1.f, 2.f };
		}
		else if (e.GetEntityID() == 2)
		{
			map = { { 6, {5, 9, 6, 5, 4, 9, 9, 1, 2} }, { 2, {0, 0, 0, 9, 6, 4} }, { 9, {8, 2, 5, 9, 6, 8} }, { 4, {2, 0, 3, 6, 9} } };
			transform->map = map;
			transform->position = { 4.f, 1.f, 5.f };
		}
	}
	
	Serializer::SerializeEntities(&gCoordinator, "test.json");
	*/

	Serializer::DeserializeJson(&gCoordinator, "test.json");

	std::cout << "\n\n\n";
	for (auto& e : gCoordinator.GetEntities())
	{
		std::cout << "\n\n";
		Transform* transform = gCoordinator.GetComponent<Transform>(e);

		std::cout << "Entity ID: " << e.GetEntityID() << "\nEntity Name: " << e.GetEntityName() << "\n";
		std::cout << "position: " << transform->position.x << ", " << transform->position.y << ", " << transform->position.z << "\n";

		std::cout << "--------------------------\n";
		for (auto k : transform->map)
		{
			std::cout << "key: " << k.first << "\n";

			std::cout << "value:";
			for (auto v : k.second)
			{
				std::cout << " " << v;
			}
			std::cout << "\n";
		}

		std::cout << "\n\n\n";
	}
	std::cout << "\n\n\n";

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