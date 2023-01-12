/******************************************************************************/
/*!
\file       Camera.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       12 January 2022
\brief
  This file contains the declaration of Camera class.

  Copyright (C) 2022 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "include/ECS/Architecture/IComponent.hpp"
#include <lib/glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
    // Determine the type of movement the camera is making
    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,

    };


	class Camera : public IComponent
    {
    public:
        Camera() : position{ 0.f, 0.f, 0.f }, front{ 0.f, 0.f, -1.f }, up{ 0.f, 1.f, 0.f }, yaw{ -90.f }, pitch{ 0.f }, movementSpeed{ 2.5f }, mouseSensitivity{ 0.1f } {}

        glm::mat4 getViewMatrix() { return glm::lookAt(position, position + front, up); }

        void processKeyboard(Camera_Movement direction, float deltaTime) {
            float velocity = movementSpeed * deltaTime;
            if (direction == FORWARD)
                position += front * velocity;
            if (direction == BACKWARD)
                position -= front * velocity;
            if (direction == LEFT)
                position -= glm::normalize(glm::cross(front, up)) * velocity;
            if (direction == RIGHT)
                position += glm::normalize(glm::cross(front, up)) * velocity;
        }

        void processMouseMovement(float xoffset, float yoffset) {
            xoffset *= mouseSensitivity;
            yoffset *= mouseSensitivity;

            yaw += xoffset;
            pitch += yoffset;

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            this->front = glm::normalize(front);
        }

        private:
            glm::vec3 position;
            glm::vec3 front;
            glm::vec3 up;
            float yaw;
            float pitch;
            float movementSpeed;
            float mouseSensitivity;
    };
}