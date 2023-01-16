// To check for memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include "include/Memory/MemoryUnitTest.hpp"
#include "include/ECS/Architecture/Coordinator.hpp"
#include "include/Serialization/Serializer.hpp"
#include "include/Tag/TagManager.hpp"

#include "include/Graphics/Shader.hpp"
#include "include/Graphics/Camera.hpp"
#include "include/ECS/System/ParticleSystem.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>

using namespace Engine;


// Forward declarations
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int windowWidth = 1280, windowHeight = 720;
double currentFrame{}, lastFrame{}, deltaTime{};
double lastX = windowWidth / 2.0, lastY = windowHeight / 2.0;

Camera camera;

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
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Particles", nullptr, nullptr);
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

	// Initialize GLEW
	glewExperimental = static_cast<GLboolean>(true); // Needed for core profile
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
		camera.setAspectRatio(width, height); // Updates window aspect ratio

		// Calculating deltaTime every frame
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// Clear every frame
		{
			glClearColor(0.f, 0.f, 0.f, 1.0f);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		
		// Setting uniforms for shaders
		{
			const auto& shd_ref_handle = shdrpgms[GraphicShader::Default].GetHandle();
			glUseProgram(shd_ref_handle);

			glm::vec3 translateVector = { 1.f, 1.f, 1.f };
			glm::vec3 scaleVector = { 1.f, 1.f, 1.f };
			glm::mat4 transformMatrix = glm::translate(translateVector) * glm::scale(scaleVector);
			glm::vec4 color = { 0.f, 1.f, 0.f, 1.f };
			//GLSLShader::SetUniform("uTransformMatrix", transformMatrix, shd_ref_handle);

			//GLSLShader::SetUniform("uViewMatrix", camera.getViewMatrix(), shd_ref_handle);
			//GLSLShader::SetUniform("uProjectionMatrix", camera.getProjectionMatrix(), shd_ref_handle);

			GLint loc = glGetUniformLocation(shd_ref_handle, "uTransformMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, &transformMatrix[0][0]);

			loc = glGetUniformLocation(shd_ref_handle, "uViewMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, &camera.getViewMatrix()[0][0]);

			loc = glGetUniformLocation(shd_ref_handle, "uProjectionMatrix");
			glUniformMatrix4fv(loc, 1, GL_FALSE, &camera.getProjectionMatrix()[0][0]);

			loc = glGetUniformLocation(shd_ref_handle, "uColor");
			glUniform4fv(loc, 1, &color[0]);

			//glUseProgram(0);
		}


		particleSystem->Update(deltaTime);


		// Resets mouse position every frame
		//glfwSetCursorPos(window, lastX, lastY);

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