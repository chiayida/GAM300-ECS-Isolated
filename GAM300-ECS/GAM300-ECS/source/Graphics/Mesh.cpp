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
#include <array>
#include <iostream>


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

    // For texture slots
    static const size_t stMaxTextures = 32;

    // Struct declaration
    struct GLMesh
    {
        glm::vec3 position{};
        glm::vec4 color{};
        glm::vec2 textureCoords{};
        float textureID{};
            
        glm::vec3 tposition{};
        glm::vec3 tscale{};
        float trotation{};
    };

    struct RendererData
    {
        GLuint va = 0; // Vertex array
        GLuint vb = 0; // Vertex buffer
        GLuint ib = 0; // Index buffer

        // First slot is for object without colors
        GLuint whitetexture = 0;
        uint32_t whitetextureslot = 0;

        uint32_t indexcount = 0;

        GLMesh* vertexbuffer = nullptr;
        GLMesh* vertexbufferptr = nullptr;

        std::array<uint32_t, stMaxTextures> arrTextureSlots{};
        uint32_t uiTextureSlotIndex = 1; // 0 = white texture (aka no texture)

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
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLMesh), (const void*)offsetof(GLMesh, textureCoords));

        glEnableVertexArrayAttrib(cubeData.va, 3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GLMesh), (const void*)offsetof(GLMesh, textureID));

        glEnableVertexArrayAttrib(cubeData.va, 4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GLMesh), (const void*)offsetof(GLMesh, tposition));

        glEnableVertexArrayAttrib(cubeData.va, 5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(GLMesh), (const void*)offsetof(GLMesh, tscale));

        glEnableVertexArrayAttrib(cubeData.va, 6);
        glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(GLMesh), (const void*)offsetof(GLMesh, trotation));

        
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

        // 1x1 white texture
        //glCreateTextures(GL_TEXTURE_2D, 1, &cubeData.whitetexture);
        glGenTextures(1, &cubeData.whitetexture);
        glBindTexture(GL_TEXTURE_2D, cubeData.whitetexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        unsigned char whitePixel[4] = { 255, 255, 255, 255 };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Set all texture slots to 0
        cubeData.arrTextureSlots[0] = cubeData.whitetexture;
        for (size_t i = 1; i < stMaxTextures; ++i)
        {
            cubeData.arrTextureSlots[i] = 0;
        }
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

        glDeleteTextures(1, &cubeData.whitetexture);

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
        for (uint32_t i = 0; i < cubeData.uiTextureSlotIndex; i++)
        {
            glBindTextureUnit(i, cubeData.arrTextureSlots[i]);
        }
        glBindVertexArray(cubeData.va);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, cubeData.indexcount, GL_UNSIGNED_INT, nullptr);
        cubeData.renderStats.drawCount++;

        cubeData.indexcount = 0;
        cubeData.uiTextureSlotIndex = 1;
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
        cubeData.vertexbufferptr->textureCoords = { 0.f, 0.f };
        cubeData.vertexbufferptr->textureID = 0.f;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;


        // front-top-right
        cubeData.vertexbufferptr->position = { 0.5f, 0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { 0.f, 0.f };
        cubeData.vertexbufferptr->textureID = 0.f;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // front-bottom-right
        cubeData.vertexbufferptr->position = { 0.5f, -0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { 0.f, 0.f };
        cubeData.vertexbufferptr->textureID = 0.f;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // front-bottom-left
        cubeData.vertexbufferptr->position = { -0.5f, -0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { 0.f, 0.f };
        cubeData.vertexbufferptr->textureID = 0.f;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-top-left
        cubeData.vertexbufferptr->position = { -0.5f, 0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { 0.f, 0.f };
        cubeData.vertexbufferptr->textureID = 0.f;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-top-right
        cubeData.vertexbufferptr->position = { 0.5f, 0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { 0.f, 0.f };
        cubeData.vertexbufferptr->textureID = 0.f;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-bottom-right
        cubeData.vertexbufferptr->position = { 0.5f, -0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { 0.f, 0.f };
        cubeData.vertexbufferptr->textureID = 0.f;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-bottom-left
        cubeData.vertexbufferptr->position = { -0.5f, -0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { 0.f, 0.f };
        cubeData.vertexbufferptr->textureID = 0.f;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        cubeData.indexcount += oneCubeIndex;
        cubeData.renderStats.cubeCount++;
    }


    // Function that adds to the vertex buffer pointer for the quad fill (with texture) mesh
    void Renderer::DrawCube(const glm::vec3& tposition, const glm::vec3 tscale, const float trotation, const uint32_t textureID, 
        const glm::vec4 color, glm::vec2 min, glm::vec2 max)
    {
        if (cubeData.indexcount >= maxCubeIndexCount)
        {
            EndCubeBatch();
            FlushCube();
            BeginCubeBatch();
        }

        float textureIndex = 0.f;
        for (uint32_t i = 1; i < cubeData.uiTextureSlotIndex; ++i)
        {
            if (cubeData.arrTextureSlots[i] == textureID)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.f)
        {
            textureIndex = (float)cubeData.uiTextureSlotIndex;
            cubeData.arrTextureSlots[(size_t)textureIndex] = textureID;
            cubeData.uiTextureSlotIndex++;
        }

        // front-top-left
        cubeData.vertexbufferptr->position = { -0.5f, 0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { min.x, max.y };
        cubeData.vertexbufferptr->textureID = textureIndex;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;


        // front-top-right
        cubeData.vertexbufferptr->position = { 0.5f, 0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { max.x, max.y };
        cubeData.vertexbufferptr->textureID = textureIndex;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // front-bottom-right
        cubeData.vertexbufferptr->position = { 0.5f, -0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { max.x, min.y };
        cubeData.vertexbufferptr->textureID = textureIndex;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // front-bottom-left
        cubeData.vertexbufferptr->position = { -0.5f, -0.5f, 0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { min.x, min.y };
        cubeData.vertexbufferptr->textureID = textureIndex;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-top-left
        cubeData.vertexbufferptr->position = { -0.5f, 0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { min.x, max.y };
        cubeData.vertexbufferptr->textureID = textureIndex;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-top-right
        cubeData.vertexbufferptr->position = { 0.5f, 0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { max.x, max.y };
        cubeData.vertexbufferptr->textureID = textureIndex;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-bottom-right
        cubeData.vertexbufferptr->position = { 0.5f, -0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { max.x, min.y };
        cubeData.vertexbufferptr->textureID = textureIndex;
        cubeData.vertexbufferptr->tposition = tposition;
        cubeData.vertexbufferptr->tscale = tscale;
        cubeData.vertexbufferptr->trotation = glm::radians(trotation);
        cubeData.vertexbufferptr++;

        // back-bottom-left
        cubeData.vertexbufferptr->position = { -0.5f, -0.5f, -0.5f };
        cubeData.vertexbufferptr->color = color;
        cubeData.vertexbufferptr->textureCoords = { min.x, min.y };
        cubeData.vertexbufferptr->textureID = textureIndex;
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