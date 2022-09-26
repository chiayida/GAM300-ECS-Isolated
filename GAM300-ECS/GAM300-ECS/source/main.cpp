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

	EntityID _1 = gCoordinator.CreateEntity(); // Entity 0
	EntityID _2 = gCoordinator.CreateEntity(); // Entity 1
	EntityID _3 = gCoordinator.CreateEntity(); // Entity 2

	for (auto& e : gCoordinator.GetEntities())
	{
		if (e.GetEntityID() == 0)
		{
			e.SetEntityName("bobby");
		}
		else if (e.GetEntityID() == 1)
		{
			e.SetEntityName("jolly");
		}
		else if (e.GetEntityID() == 2)
		{
			e.SetEntityName("barny");
		}

		gCoordinator.AddComponent<Transform>(e);
		Transform* transform = gCoordinator.GetComponent<Transform>(e);
		
		if (e.GetEntityID() == 0)
		{
			glm::mat4 m2{};

			transform->vMatrix.push_back(m2);
		}
		else if (e.GetEntityID() == 1)
		{
			glm::mat4 m2{};

			m2[0][0] = 1; m2[0][1] = 2.f; m2[0][2] = 3.f; m2[0][3] = 4.f;
			m2[1][0] = 1; m2[1][1] = 2.f; m2[1][2] = 3.f; m2[1][3] = 4.f;
			m2[2][0] = 1; m2[2][1] = 2.f; m2[2][2] = 3.f; m2[2][3] = 4.f;
			m2[3][0] = 1; m2[3][1] = 2.f; m2[3][2] = 3.f; m2[3][3] = 4.f;

			transform->vMatrix.push_back(m2);
		}
		else
		{
			glm::mat4 m2{};

			m2[0][0] = 4.; m2[0][1] = 3.f; m2[0][2] = 2.f; m2[0][3] = 1.f;
			m2[1][0] = 4.; m2[1][1] = 3.f; m2[1][2] = 2.f; m2[1][3] = 1.f;
			m2[2][0] = 4.; m2[2][1] = 3.f; m2[2][2] = 2.f; m2[2][3] = 1.f;
			m2[3][0] = 4.; m2[3][1] = 3.f; m2[3][2] = 2.f; m2[3][3] = 1.f;

			transform->vMatrix.push_back(m2);

			m2 = {};

			m2[0][0] = 12; m2[0][1] = 34; m2[0][2] = 56; m2[0][3] = 78;
			m2[1][0] = 21; m2[1][1] = 43; m2[1][2] = 65; m2[1][3] = 87;
			m2[2][0] = 32; m2[2][1] = 54; m2[2][2] = 76; m2[2][3] = 98;
			m2[3][0] = 23; m2[3][1] = 45; m2[3][2] = 67; m2[3][3] = 89;

			transform->vMatrix.push_back(m2);
		}

		/*
		for (int i = 0; i < transform->vMatrix.size(); i++)
		{
			std::cout << ": \n";
			std::cout << transform->vMatrix[i][0][0] << " " << transform->vMatrix[i][0][1] << " " << transform->vMatrix[i][0][2] << " " << transform->vMatrix[i][0][3] << "\n"
					  << transform->vMatrix[i][1][0] << " " << transform->vMatrix[i][1][1] << " " << transform->vMatrix[i][1][2] << " " << transform->vMatrix[i][1][3] << "\n"
					  << transform->vMatrix[i][2][0] << " " << transform->vMatrix[i][2][1] << " " << transform->vMatrix[i][2][2] << " " << transform->vMatrix[i][2][3] << "\n"
					  << transform->vMatrix[i][3][0] << " " << transform->vMatrix[i][3][1] << " " << transform->vMatrix[i][3][2] << " " << transform->vMatrix[i][3][3] << "\n"
					  << "\n\n";
		}
		*/
	}

	Serializer::SerializeEntities(&gCoordinator, "test.json");

	gCoordinator.DestroyEntity(_1);
	gCoordinator.DestroyEntity(_2);
	gCoordinator.DestroyEntity(_3);

	Serializer::DeserializeJson(&gCoordinator, "test.json");

	std::cout << "\n\n\n\n";

	for (auto& e : gCoordinator.GetEntities())
	{
		std::cout << "id: " << e.GetEntityID() << "\n";


		Transform* transform = gCoordinator.GetComponent<Transform>(e);
		for (int i = 0; i < transform->vMatrix.size(); i++)
		{
			std::cout << "asdasd: \n";
			std::cout << transform->vMatrix[i][0][0] << " " << transform->vMatrix[i][0][1] << " " << transform->vMatrix[i][0][2] << " " << transform->vMatrix[i][0][3] << "\n"
				<< transform->vMatrix[i][1][0] << " " << transform->vMatrix[i][1][1] << " " << transform->vMatrix[i][1][2] << " " << transform->vMatrix[i][1][3] << "\n"
				<< transform->vMatrix[i][2][0] << " " << transform->vMatrix[i][2][1] << " " << transform->vMatrix[i][2][2] << " " << transform->vMatrix[i][2][3] << "\n"
				<< transform->vMatrix[i][3][0] << " " << transform->vMatrix[i][3][1] << " " << transform->vMatrix[i][3][2] << " " << transform->vMatrix[i][3][3] << "\n"
				<< "\n\n";
		}
	}



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