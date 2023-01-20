/* Start Header**********************************************************************************/
/*
@file    Mesh.hpp
@author  Chia Yi Da		c.yida@digipen.edu
@date    16/06/2021
\brief
This file contains the declaration of function that sets up the dynamic buffers of vertices and
ebo for batch rendering.


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **********************************************************************************/
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

namespace Engine
{
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void DrawCube(const glm::vec3& tposition, const glm::vec3 tscale, const float trotation, const glm::vec4 color);
        static void DrawCube(const glm::vec3& tposition, const glm::vec3 tscale, const float trotation, const uint32_t textureID, const glm::vec4 color = { 1.f, 1.f, 1.f, 1.f }, glm::vec2 min = { 0.f, 0.f }, glm::vec2 max = { 1.f, 1.f });
        static void BeginCubeBatch();
        static void EndCubeBatch();
        static void FlushCube();

        // Struct declaration
        struct Stats
        {
            uint32_t drawCount = 0;
            uint32_t cubeCount = 0;
        };

        // Getter functions for stats
        static const Stats& GetCubeStats();
        static void ResetStats();

    private:
        static void InitCube();
        static void DestroyCube();
        static void ResetCubeStats();
    };
}