/* Start Header**********************************************************************************/
/*
@file    Mesh.cpp
@author  Chia Yi Da		c.yida@digipen.edu  	100%
@date    16/06/2021
\brief
This file contains the definition of function that sets up the dynamic buffers of vertices and
ebo for batch rendering.


Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **********************************************************************************/
#include "include/Graphics/Mesh.hpp"

#define nextCubeDataBufferPtr(x, y, z) cubeData.vertexbufferptr->position = { x, y, z };\
                                       cubeData.vertexbufferptr->color = color;\
                                       cubeData.vertexbufferptr->tposition = tposition;\
                                       cubeData.vertexbufferptr->tscale = tscale;\
                                       cubeData.vertexbufferptr->trotation = glm::radians(trotation);\
                                       cubeData.vertexbufferptr++;

namespace Engine
{
    // Change to what is best suited
    #define MAXCUBECOUNT 100  // How many cubes per buffer

    // Initialise variables

    // For cubes; fill
    static const size_t maxCubeCount = MAXCUBECOUNT;
    static const size_t oneCubeVertex = 8;
    static const size_t oneCubeIndex = 36;
    static const size_t maxCubeVertexCount = maxCubeCount * oneCubeVertex;
    static const size_t maxCubeIndexCount = maxCubeCount * oneCubeIndex;

    // Struct declaration
    struct GLMesh
    {
        glm::vec3 position{};
        glm::vec4 color{};
            
        glm::vec3 tposition{};
        glm::vec3 tscale{};
        float trotation{};
    };

    struct RendererData
    {
        GLuint va = 0; // Vertex array
        GLuint vb = 0; // Vertex buffer
        GLuint ib = 0; // Index buffer

        uint32_t indexcount = 0;

        GLMesh* vertexbuffer = nullptr;
        GLMesh* vertexbufferptr = nullptr;

        Renderer::Stats renderStats;
    };

    static RendererData cubeData;


    // Init function for Quad fill mesh
    void Renderer::InitCube()
    {
        if (cubeData.vertexbuffer != nullptr)
        {
            return;
        }
        cubeData.vertexbuffer = new GLMesh[maxCubeVertexCount];

        glCreateVertexArrays(1, &cubeData.va);
        glBindVertexArray(cubeData.va);

        glCreateBuffers(1, &cubeData.vb);
        glBindBuffer(GL_ARRAY_BUFFER, cubeData.vb);
        glBufferData(GL_ARRAY_BUFFER, maxCubeVertexCount * sizeof(GLMesh), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexArrayAttrib(cubeData.va, 0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLMesh), (const void*)offsetof(GLMesh, position));

        glEnableVertexArrayAttrib(cubeData.va, 1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLMesh), (const void*)offsetof(GLMesh, color));

        glEnableVertexArrayAttrib(cubeData.va, 2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLMesh), (const void*)offsetof(GLMesh, tposition));

        glEnableVertexArrayAttrib(cubeData.va, 3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLMesh), (const void*)offsetof(GLMesh, tscale));

        glEnableVertexArrayAttrib(cubeData.va, 4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GLMesh), (const void*)offsetof(GLMesh, trotation));

        
        uint32_t indices[maxCubeIndexCount]{};
        uint32_t offset = 0;

        for (int i = 0; i < maxCubeIndexCount; i += oneCubeIndex)
        {
            // Front face
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;
            indices[i + 3] = 0 + offset;
            indices[i + 4] = 2 + offset;
            indices[i + 5] = 3 + offset;

            // Right face
            indices[i + 6] = 1 + offset;
            indices[i + 7] = 5 + offset;
            indices[i + 8] = 6 + offset;
            indices[i + 9] = 1 + offset;
            indices[i + 10] = 6 + offset;
            indices[i + 11] = 2 + offset;

            // Back face
            indices[i + 12] = 7 + offset;
            indices[i + 13] = 6 + offset;
            indices[i + 14] = 5 + offset;
            indices[i + 15] = 7 + offset;
            indices[i + 16] = 5 + offset;
            indices[i + 17] = 4 + offset;

            // Left face
            indices[i + 18] = 4 + offset;
            indices[i + 19] = 0 + offset;
            indices[i + 20] = 3 + offset;
            indices[i + 21] = 4 + offset;
            indices[i + 22] = 3 + offset;
            indices[i + 23] = 7 + offset;

            // Top face
            indices[i + 24] = 4 + offset;
            indices[i + 25] = 5 + offset;
            indices[i + 26] = 1 + offset;
            indices[i + 27] = 4 + offset;
            indices[i + 28] = 1 + offset;
            indices[i + 29] = 0 + offset;

            // Bottom face
            indices[i + 30] = 3 + offset;
            indices[i + 31] = 2 + offset;
            indices[i + 32] = 6 + offset;
            indices[i + 33] = 3 + offset;
            indices[i + 34] = 6 + offset;
            indices[i + 35] = 7 + offset;

            offset += oneCubeVertex;
        }

        glCreateBuffers(1, &cubeData.ib);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeData.ib);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }


    // Init function for Renderer; Calls all init meshes functions
    void Renderer::Init()
    {
        InitCube();
    }

    // Shutdown function for Renderer; Deletes all allocated memory for all meshes
    void Renderer::DestroyCube()
    {
        glDeleteVertexArrays(1, &cubeData.va);
        glDeleteBuffers(1, &cubeData.vb);
        glDeleteBuffers(1, &cubeData.ib);

        delete[] cubeData.vertexbuffer;
    }


    // For GraphicSystem
    void Renderer::Shutdown()
    {
        DestroyCube();
    }


    // BeginBatch functions assigns the vertexbufferptr to the start of the buffer
    void Renderer::BeginCubeBatch()
    {
        cubeData.vertexbufferptr = cubeData.vertexbuffer;
    }


    // EndBatch functions binds the respective buffers by getting the offset
    void Renderer::EndCubeBatch()
    {
        GLsizeiptr sizeQuad = (uint8_t*)cubeData.vertexbufferptr - (uint8_t*)cubeData.vertexbuffer;
        glBindBuffer(GL_ARRAY_BUFFER, cubeData.vb);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeQuad, cubeData.vertexbuffer);
    }


    // Function that binds texture slots followed by batch rendering the VA
    void Renderer::FlushCube()
    {
        glBindVertexArray(cubeData.va);

        glDrawElements(GL_TRIANGLES, cubeData.indexcount, GL_UNSIGNED_INT, nullptr);
        cubeData.renderStats.drawCount++;

        cubeData.indexcount = 0;
    }


    // Function that adds to the vertex buffer pointer for the quad fill (without texture) mesh
    void Renderer::DrawCube(const glm::vec3& tposition, const glm::vec3 tscale, const float trotation, const glm::vec4 color)
    {
        if (cubeData.indexcount >= maxCubeIndexCount)
        {
            EndCubeBatch();
            FlushCube();
            BeginCubeBatch();
        }

        // front-top-left
        cubeData.vertexbufferptr->position = { -0.5f, 0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;


        // front-top-right
        cubeData.vertexbufferptr->position = { 0.5f, 0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // front-bottom-right
        cubeData.vertexbufferptr->position = { 0.5f, -0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // front-bottom-left
        cubeData.vertexbufferptr->position = { -0.5f, -0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-top-left
        cubeData.vertexbufferptr->position = { -0.5f, 0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-top-right
        cubeData.vertexbufferptr->position = { 0.5f, 0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-bottom-right
        cubeData.vertexbufferptr->position = { 0.5f, -0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-bottom-left
        cubeData.vertexbufferptr->position = { -0.5f, -0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        cubeData.indexcount += oneCubeIndex;
        cubeData.renderStats.cubeCount++;
    }


    // Functions that get the render stats
    const Renderer::Stats& Renderer::GetCubeStats()
    {
        return cubeData.renderStats;
    }


    // Functions that resets the render stats
    void Renderer::ResetCubeStats()
    {
        memset(&cubeData.renderStats, 0, sizeof(Stats));
    }


    // For GraphicSystem
    void Renderer::ResetStats()
    {
        ResetCubeStats();
    }
}