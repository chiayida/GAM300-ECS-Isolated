/******************************************************************************/
/*!
\file       MemoryUnitTest.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       11 July 2022
\brief
  This file contains the declaration and definition of TestComponent class.
  It also contains functions that is to be called to check whether allocators
  are working as intended.

  CustomAllocator_Test() REQUIRES MANUAL CHECKS!!!

  - FreeListAllocator_Test()
  - CustomAllocator_Test()

  When #include header to call Unit Test functions, do remember to comment out 
  the header file for Coordinator as there will be redefinition of classes.
*/
/******************************************************************************/
#pragma once

#include "Allocator.hpp"
#include "FreeListAllocator.hpp"
#include "CustomAllocator.hpp"

#include <iostream>
#include <cstddef>
#include <vector>
#include <string>

namespace Engine
{
    // Test class used for custom allocator testing. 
    // size = 80 -> padding + size = 11 + 69 (11 padding -> +4 for int, 7 for bool, padding in 8s)
    class TestComponent
    {
    public:
        TestComponent(int ID) : textureID{ ID } {};

        ~TestComponent()
        {
            textureID = 0;
        }

        // 4 + 32 + 32 + 1 = 69 (size)
        int textureID;

        std::vector<float> uv{};
        std::vector<float> rgb{};

        bool flag = true;
    };


    // Forward function declarations
    void FreeListAllocator_Test();
    void CustomAllocator_Test();

    void FreeListAllocator_Test1(std::string s1);
    void FreeListAllocator_Test2(std::string s1, std::string s2);
    void FreeListAllocator_Test3(std::string s1, std::string s2, std::string s3);
    void FreeListAllocator_Test4(std::string s1, std::string s2, std::string s3);

    template <typename T>
    void CustomAllocator_Test1();


    // Test case 1 (Allocation (Component Constructor), Freeing (Component Destructor))
    // 2 MANUAL CHECKS REQUIRED. 
    template <typename T>
    void CustomAllocator_Test1<T>()
    {
        std::cout << "==============================================================" << std::endl;
        std::cout << "Custom Allocator @ Test 1" << std::endl;
        std::cout << "==============================================================" << std::endl;

        bool flag1 = false, flag2 = false, flag3 = false, flag4 = false;

        // Add a huge chunk of memory at start of application and initialise the allocators.
        Allocator* freeListAllocator = new FreeListAllocator((size_t)1e8);
        CustomAllocator<T, 10>* tempCompArray = new CustomAllocator<T, 10>(freeListAllocator); // size is 816


        // Calls T constructor and create new component. (TextureID is set to 100)
        // Check whether textureID is same as allocated. If same, means allocate function is working.
        T* pTexture = tempCompArray->Allocate(100);
        if (pTexture->textureID == 100) flag1 = true;
        std::cout << "ALLOCATE - pTexture->textureID: " << pTexture->textureID << std::endl;


        // Calls T destructor and deletes component. (TextureID is set to 0)
        // Check whether textureID is same as freed. If same, means free function is working.
        tempCompArray->Free(pTexture);
        if (pTexture->textureID == 0) flag2 = true;
        std::cout << "FREE - pTexture->textureID: " << pTexture->textureID << std::endl;

        // Same thing as before, this time it is to check whether memory used in PoolAllocator is still 80.
        // Need to MANUALLY check through cmd. If it is not 80 means allocation or free function is wrong.
        // Commenting out "tempCompArray->Free(pTexture);" above should give u a memory used value of 160 (80 + 80)
        T* pTexture1 = tempCompArray->Allocate(1100);
        tempCompArray->Free(pTexture1);


        // This time, check whether FreeListAllocator's memory used is 8016.
        // Need to MANUALLY check through cmd. If it is not 8016 means PoolListAllocator destructor for Custom Allocator is wrong.
        // Commenting out "delete tempCompArray;" below should give u a memory used value of 8832 (8016 + 816)
        delete tempCompArray;
        CustomAllocator<T, 100>* tempCA = new CustomAllocator<T, 100>(freeListAllocator);


        // Final Checks, check whether allocation and free still works with new ComponentArray
        T* pTexture2 = tempCA->Allocate(1234);
        if (pTexture2->textureID == 1234) flag3 = true;
        std::cout << "ALLOCATE - pTexture2->textureID: " << pTexture2->textureID << std::endl;

        tempCA->Free(pTexture2);
        if (pTexture2->textureID == 0) flag4 = true;
        std::cout << "FREE - pTexture2->textureID: " << pTexture2->textureID << std::endl;

        delete tempCA;
        delete freeListAllocator;


        std::cout << "==============================================================" << std::endl;

        if (flag1 && flag2 && flag3 && flag4) std::cout << "TEST 1: PASS" << std::endl;
        else std::cout << "TEST 1: FAIL" << std::endl;
        std::cout << std::endl << std::endl << std::endl << std::endl;


    }

} // end of namespace