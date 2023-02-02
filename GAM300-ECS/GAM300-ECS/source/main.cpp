// To check for memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include "include/Memory/MemoryUnitTest.hpp"
#include "include/ECS/Architecture/Coordinator.hpp"
#include "include/Serialization/Serializer.hpp"
#include "include/Tag/TagManager.hpp"

#include "include/Graphics/ResourceManager.hpp"
#include "include/Graphics/ModelManager.hpp"
#include "include/Graphics/Shader.hpp"

#include "include/Graphics/Camera.hpp"
#include "include/ECS/Component/Transform.hpp"
#include "include/ECS/Component/Particle.hpp"
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

int windowWidth = 1920, windowHeight = 1080;
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
	// Load Default shader program
	const auto& shd_ref_handle = shdrpgms[GraphicShader::Default].GetHandle();
	glUseProgram(shd_ref_handle);

	// Uniform for game object texture shader
	auto loc = glGetUniformLocation(shd_ref_handle, "uTextures");
	int samplers[32]{};
	for (int i = 0; i < 32; i++)
	{
		samplers[i] = i;
	}
	glUniform1iv(loc, 32, samplers);

	// Unload shader program
	glUseProgram(0);

	auto particleSystem = gCoordinator.GetSystem<ParticleSystem>();
	particleSystem->Init();


	ResourceManager gResourceManager{};
	gResourceManager.Create();
	gResourceManager.LoadTexture("Map.png");

	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////

	EntityID entity0 = gCoordinator.CreateEntity();

	gCoordinator.AddComponent<Transform>(entity0);


	bool isLooping = true;
	bool isRotate = true;
	int maxParticles = 10;
	float gravityModifier = 0.f;
	float radius = 1.f;
	float rotationSpeed = 20.f;
	glm::vec4 startColor = { 1.f, 0.682f, 0.259f, 1.f };
	glm::vec4 endColor = { 1.f, 1.f, 1.f, 1.f };
	glm::vec3 minSpeed = { -0.f, -0.f, -0.f };
	glm::vec3 maxSpeed = { 0.f, 0.5f, 0.f };
	glm::vec3 minSize = { 0.05f, 0.05f, 0.05f };
	glm::vec3 maxSize = { 0.05f, 0.05f, 0.05f };
	float minLifespan = 1.f;
	float maxLifespan = 3.f;
	bool isSphere = false;
	bool isCone = true;

	gCoordinator.AddComponent<Particle>(entity0, 
		Particle(isLooping, isRotate, maxParticles, gravityModifier, radius, rotationSpeed,
				 startColor, endColor, minSpeed, maxSpeed, minSize, maxSize,
				 minLifespan, maxLifespan, isSphere, isCone));

	glm::vec3 position = { 0.f, 0.f, -3.f };
	Particle* particle = gCoordinator.GetComponent<Particle>(entity0);
	particle->Init(position);

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

		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		{
			particle->isSphere = false;
			particle->isCone = false;
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			particle->isSphere = true;
			particle->isCone = false;
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			particle->isSphere = false;
			particle->isCone = true;
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			particle->Init(position);
			particle->isLooping = false;
		}
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			particle->Init(position);
			particle->isLooping = true;
		}

		particleSystem->Update(&gCoordinator, deltaTime);

		// Resets mouse position every frame
		glfwSetCursorPos(window, lastX, lastY);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	particleSystem->Destroy();
	gResourceManager.Destroy();

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