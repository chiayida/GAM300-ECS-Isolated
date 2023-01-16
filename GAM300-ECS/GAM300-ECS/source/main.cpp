// To check for memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include "include/Memory/MemoryUnitTest.hpp"
#include "include/ECS/Architecture/Coordinator.hpp"
#include "include/Serialization/Serializer.hpp"
#include "include/Tag/TagManager.hpp"

#include "include/Graphics/Camera.hpp"
#include "include/Graphics/shader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace Engine;


// Forward declarations
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int windowWidth = 1280, windowHeight = 720;
double currentFrame{}, lastFrame{}, deltaTime{};
double lastX = windowWidth / 2.0, lastY = windowHeight / 2.0;

Camera camera;
GLuint shaderHandle = LoadShaders("GAM300-ECS/Assets/Default.vert", 
							   "GAM300-ECS/Assets/Default.frag");


int main()
{
	//////////////////////////////////////////
	// Memory leak check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialise GLFW
	if (!glfwInit())
	{
		LOG_WARNING("Failed to initialize GLFW");
		return -1;
	}
	
	// Initialise Application
	TagManager gTagManager;
	Serializer::DeserializeLayers(&gTagManager, "layers.layer");
	Serializer::DeserializeTags(&gTagManager, "tags.tag");

	Coordinator gCoordinator;
	gCoordinator.Init();
	Serializer::DeserializeJson(&gCoordinator, &gTagManager, "test.scene");
	//////////////////////////////////////////

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window;
	window = glfwCreateWindow(windowWidth, windowHeight, "Particles", nullptr, nullptr);
	if (!window)
	{
		LOG_WARNING("Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Disable cursor when window is in focus
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
	glfwSetCursorPosCallback(window, mouse_callback);


	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Objects in viewport scale according to window width and height
		int width = 0, height = 0;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		camera.setAspectRatio(width, height); // Updates window aspect ratio

		// Calculating deltaTime every frame
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// Render here


		// Clear every frame
		{
			glClearColor(0.41f, 0.41f, 0.41f, 1.f);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}




		// Resets mouse position every frame
		glfwSetCursorPos(window, lastX, lastY);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 1;
}


// Keyboard input
void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
	{
		camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
	{
		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
	{
		camera.processKeyboard(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
	{
		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.processKeyboard(CameraMovement::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.processKeyboard(CameraMovement::DOWN, deltaTime);
	}
}


// Mouse input
void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}