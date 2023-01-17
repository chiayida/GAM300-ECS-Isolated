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

// Only objects between the near and far planes will be rendered
const float NEAR = 0.1f;
const float FAR = 100.0f;

namespace Engine
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(2.5f), mouseSensitivity(0.1f), fov{ 45.f }
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }


    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }


    glm::mat4 Camera::getProjectionMatrix() 
    {
        return glm::perspective(glm::radians(fov), aspectRatio, NEAR, FAR);
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
            position -= up * velocity;
        }
    }


    void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // When pitch is out of bounds, screen does not flip
        if (constrainPitch)
        {
            if (pitch > 89.0f)
            {
                pitch = 89.0f;
            }
            if (pitch < -89.0f)
            {
                pitch = -89.0f;
            }
        }

        updateCameraVectors();
    }


    void Camera::processMouseScroll(float yoffset) 
    {
        // Change the field of view (FOV) based on scroll input
        if (fov >= 1.0f && fov <= 45.0f)
        {
            fov -= yoffset;
        }
        if (fov <= 1.0f)
        {
            fov = 1.0f;
        }
        if (fov >= 45.0f)
        {
            fov = 45.0f;
        }
    }


    void Camera::updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);

        right = glm::normalize(glm::cross(this->front, worldUp));
        up = glm::normalize(glm::cross(right, this->front));
    }
}