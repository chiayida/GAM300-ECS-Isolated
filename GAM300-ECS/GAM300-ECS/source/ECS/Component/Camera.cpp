/******************************************************************************/
/*!
\file       Camera.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       12 January 2023
\brief
  This file contains the implementation of Camera class.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
#include "include/Graphics/Camera.hpp"

#define NEAR 0.1f
#define FAR 100.f

namespace Engine
{
    Camera::Camera() : position{ 0.f, 0.f, -1.f }, front{ 0.f, 0.f, -1.f }, up{ 0.f, 1.f, 0.f }, yaw{ -90.f }, pitch{ 0.f }, 
        movementSpeed{ 2.5f }, mouseSensitivity{ 0.1f }, zoom{ 45.f }, aspectRatio{ 0.f } {}


    void Camera::setAspectRatio(int width, int height)
    {
        aspectRatio = (float)width / (float)height;
    }


    glm::mat4 Camera::getViewMatrix() 
    { 
        return glm::lookAt(position, position + front, up); 
    }


    glm::mat4 Camera::getProjectionMatrix()
    {
        return glm::perspective(glm::radians(zoom), aspectRatio, NEAR, FAR);
    }


    void Camera::processKeyboard(CameraMovement direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;

        if (direction == CameraMovement::FORWARD)
        {
            position += front * velocity;
        }
        if (direction == CameraMovement::BACKWARD)
        {
            position -= front * velocity;
        }
        if (direction == CameraMovement::LEFT)
        {
            position -= glm::normalize(glm::cross(front, up)) * velocity;
        }
        if (direction == CameraMovement::RIGHT)
        {
            position += glm::normalize(glm::cross(front, up)) * velocity;
        }
        if (direction == CameraMovement::UP)
        {
            position += up * velocity;
        }
        if (direction == CameraMovement::DOWN)
        {
            position -= glm::normalize(glm::cross(front, up)) * velocity;
        }
    }


    void Camera::processMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // When pitch is out of bounds, screen does not flip
        if (pitch > 89.0f)
        {
            pitch = 89.0f;
        }
        if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }

        // Update Camera vectors
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);
    }
}