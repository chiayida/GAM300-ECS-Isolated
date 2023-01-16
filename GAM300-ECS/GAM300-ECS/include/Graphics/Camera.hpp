/******************************************************************************/
/*!
\file       Camera.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       12 January 2023
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
        UP,
        DOWN
    };


	class Camera
    {
    public:
        Camera();
        ~Camera() = default;

        void setAspectRatio(int width, int height);
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();

        void processKeyboard(CameraMovement direction, float deltaTime);
        void processMouseMovement(float xoffset, float yoffset);



        private:
            glm::vec3 position;
            glm::vec3 front;
            glm::vec3 up;

            float yaw;
            float pitch;
            float movementSpeed;
            float mouseSensitivity;
            float zoom;
            float aspectRatio{};
    };
}