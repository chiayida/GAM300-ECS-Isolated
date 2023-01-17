/******************************************************************************/
/*!
\file       Camera.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       12 January 2023
\brief
  This file contains the declaration of Camera class.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    // Determine the type of movement the camera is making
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    // Camera class
    class Camera
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();

        void processKeyboard(CameraMovement direction, float deltaTime);
        void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
        void processMouseScroll(float yoffset);

        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        float yaw;
        float pitch;

        float movementSpeed;
        float mouseSensitivity;

        float fov;
        float aspectRatio;

    private:
        void updateCameraVectors();
    };
}