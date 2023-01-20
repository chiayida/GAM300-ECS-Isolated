// To check for memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include "include/Memory/MemoryUnitTest.hpp"
#include "include/ECS/Architecture/Coordinator.hpp"
#include "include/Serialization/Serializer.hpp"
#include "include/Tag/TagManager.hpp"

#include "include/Graphics/ModelManager.hpp"
#include "include/Graphics/Shader.hpp"

#include "include/Graphics/Camera.hpp"
#include "include/ECS/System/ParticleSystem.hpp"
#include "include/Graphics/Mesh.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Engine;

// Function prototypes
void processInput(GLFWwindow* window_);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

int windowWidth = 1280, windowHeight = 720;
double currentFrame{}, lastFrame{}, deltaTime{};

Camera camera;
double lastX = windowWidth / 2.0, lastY = windowHeight / 2.0;
bool firstMouse = true;


int main()
{
	// Memory leak check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 

	// Initialise Application

	// Initialise GLFW
	if (!glfwInit())
	{
		LOG_WARNING("Failed to initialize GLFW");
		return -1;
	}

	// Setting up OpenGL properties
	glfwWindowHint(GLFW_SAMPLES, 1); // change for anti-aliasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Camera Test", nullptr, nullptr);
	if (!window)
	{
		LOG_WARNING("Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	// Disable cursor when window is in focus
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		LOG_WARNING("Failed to initialize GLEW");
		glfwTerminate();
		return -1;
	}

	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	TagManager gTagManager;
	Serializer::DeserializeLayers(&gTagManager, "layers.layer");
	Serializer::DeserializeTags(&gTagManager, "tags.tag");

	Coordinator gCoordinator;
	gCoordinator.Init();
	Serializer::DeserializeJson(&gCoordinator, &gTagManager, "test.scene");

	ShaderSetup();

	auto particleSystem = gCoordinator.GetSystem<ParticleSystem>();
	particleSystem->Init();

	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Objects in viewport scale according to window width and height
		int width = 0, height = 0;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		camera.aspectRatio = (float)width / (float)height;

		// Calculating deltaTime every frame
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// Clear screen
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		// Setting uniforms for shaders
		{
			const auto& shd_ref_handle = shdrpgms[GraphicShader::Default].GetHandle();
			glUseProgram(shd_ref_handle);

			GLint loc = glGetUniformLocation(shd_ref_handle, "uViewMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));

			loc = glGetUniformLocation(shd_ref_handle, "uProjectionMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
		}

		particleSystem->Update(deltaTime);

		// Resets mouse position every frame
		glfwSetCursorPos(window, lastX, lastY);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	particleSystem->Destroy();
	glfwTerminate();

	return 1;
}


void processInput(GLFWwindow* window_)
{
	if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window_, GL_TRUE);
	}

	if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyboard(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.processKeyboard(CameraMovement::UP, deltaTime);
	}
	if (glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.processKeyboard(CameraMovement::DOWN, deltaTime);
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) 
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) 
{
	camera.processMouseScroll(yoffset);
}