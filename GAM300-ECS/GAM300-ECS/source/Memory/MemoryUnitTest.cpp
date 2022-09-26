/******************************************************************************/
/*!
\file       MemoryUnitTest.cpp
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
*/
/******************************************************************************/
#pragma once

#include "include/Memory/MemoryUnitTest.hpp"

namespace Engine
{
    /******************************************************************************/
    /*!
        FreeListAllocator Test cases function implementations.
    */
    /******************************************************************************/

    // Call function to test whether Free List Allocator is working as intented.
    // Test cases for FreeList Allocator.
    void FreeListAllocator_Test()
    {
        FreeListAllocator_Test1("TEST 1");
        FreeListAllocator_Test2("HELLO WORLD!", "TEST 2");
        FreeListAllocator_Test3("HELLO WORLD!", "HUAT STUDIOS", "TEST 3");
        FreeListAllocator_Test4("HELLO WORLD!", "HUAT STUDIOS", "TEST 4");

        std::cout << "Free List Allocator is working if it passes all 4 tests" << std::endl;
    }

    // Test case 1 (Allocation and Writing of memory data)
    void FreeListAllocator_Test1(std::string s1)
    {
        std::cout << "==============================================================" << std::endl;
        std::cout << "Free List Allocator @ Test 1" << std::endl;
        std::cout << "==============================================================" << std::endl;

        // Add a huge chunk of memory at start of application and initialise the allocator.
        Allocator* freeListAllocator = new FreeListAllocator((size_t)1e8, FreeListAllocator::Policy::FIND_FIRST);

        // Allocating a small chunk of memory.
        void* strPtr = freeListAllocator->Allocate(16, 8);
        std::cout << std::endl << "Allocated memory address: @D " << strPtr << std::endl;

        // Writing of string param to memory.
        strcpy(reinterpret_cast<char*>(strPtr), s1.c_str());

        // Iterate through allocated memory and get its data.
        std::string str{};
        for (int i = 0; i < s1.length(); i++)
        {
            std::size_t it = (std::size_t)strPtr + i;
            str += *(char*)it;
        }
        std::cout << "s1:   " << s1 << std::endl << "data: " << str << std::endl << std::endl;

        delete freeListAllocator;

        std::cout << "==============================================================" << std::endl;

        if (str == s1) std::cout << "TEST 1: PASS" << std::endl;
        else std::cout << "TEST 1: FAIL" << std::endl;

        std::cout << std::endl << std::endl << std::endl << std::endl;
    }


    // Test case 2 (Allocation, Writing, Freeing, Reallocation and Rewriting of memory)
    // Only 1 instance of allocated memory. 
    // Therefore, reallocated memory address should match first allocated memory address.
    void FreeListAllocator_Test2(std::string s1, std::string s2)
    {
        std::cout << "==============================================================" << std::endl;
        std::cout << "Free List Allocator @ Test 2" << std::endl;
        std::cout << "==============================================================" << std::endl;

        // Add a huge chunk of memory at start of application and initialise the allocator.
        Allocator* freeListAllocator = new FreeListAllocator((size_t)1e8, FreeListAllocator::Policy::FIND_FIRST);

        // Allocating a small chunk of memory.
        void* strPtr1 = freeListAllocator->Allocate(16, 8);
        std::cout << std::endl << "Allocated memory address: @D " << strPtr1 << std::endl;

        // Writing of string param to memory.
        strcpy(reinterpret_cast<char*>(strPtr1), s1.c_str());

        // Iterate through allocated memory and get its data.
        std::string str1{};
        for (int i = 0; i < s1.length(); i++)
        {
            std::size_t it = (std::size_t)strPtr1 + i;
            str1 += *(char*)it;
        }
        std::cout << "s1:   " << s1 << std::endl << "data: " << str1 << std::endl << std::endl;

        // 'Free' allocated (Move it back to free list, to be reused)
        freeListAllocator->Free(strPtr1);

        // Reallocate same memory size to pointer.
        void* strPtr2 = freeListAllocator->Allocate(16, 8);
        std::cout << std::endl << "Reallocated memory address: @D " << strPtr2 << std::endl;

        strcpy(reinterpret_cast<char*>(strPtr2), s2.c_str());

        std::string str2{};
        for (int i = 0; i < s2.length(); i++)
        {
            std::size_t it = (std::size_t)strPtr2 + i;
            str2 += *(char*)it;
        }
        std::cout << "s2:   " << s2 << std::endl << "data: " << str2 << std::endl << std::endl;

        delete freeListAllocator;

        std::cout << "==============================================================" << std::endl;

        if (str1 == s1 && str2 == s2 && strPtr1 == strPtr2) std::cout << "TEST 2: PASS" << std::endl;
        else std::cout << "TEST 2: FAIL" << std::endl;

        std::cout << std::endl << std::endl << std::endl << std::endl;
    }


    // Test case 3 (Allocation, Writing, Freeing, Reallocation and Rewriting of memory)
    // 2 instances of memory allocation before first allocated memory data is freed.
    // Reallocated memory < first instance of memory allocation. Memory address CAN be reused as size fits.
    // Therefore, reallocated memory address SHOULD match first allocated memory address.
    void FreeListAllocator_Test3(std::string s1, std::string s2, std::string s3)
    {
        std::cout << "==============================================================" << std::endl;
        std::cout << "Free List Allocator @ Test 3" << std::endl;
        std::cout << "==============================================================" << std::endl;

        // Add a huge chunk of memory at start of application and initialise the allocator.
        Allocator* freeListAllocator = new FreeListAllocator((size_t)1e8, FreeListAllocator::Policy::FIND_FIRST);


        // First Instance


        // Allocating a small chunk of memory
        void* strPtr1 = freeListAllocator->Allocate(24, 8);
        std::cout << std::endl << "Allocated memory address 1: @D " << strPtr1 << std::endl;

        // Writing of string param to memory.
        strcpy(reinterpret_cast<char*>(strPtr1), s1.c_str());

        // Iterate through allocated memory and get its data.
        std::string str1{};
        for (int i = 0; i < s1.length(); i++)
        {
            std::size_t it = (std::size_t)strPtr1 + i;
            str1 += *(char*)it;
        }
        std::cout << "s1:   " << s1 << std::endl << "data: " << str1 << std::endl << std::endl;


        // Second Instance


        void* strPtr2 = freeListAllocator->Allocate(24, 8);
        std::cout << std::endl << "Allocated memory address 2: @D " << strPtr2 << std::endl;

        strcpy(reinterpret_cast<char*>(strPtr2), s2.c_str());

        // Iterate through allocated memory and get its data.
        std::string str2{};
        for (int i = 0; i < s2.length(); i++)
        {
            std::size_t it = (std::size_t)strPtr2 + i;
            str2 += *(char*)it;
        }
        std::cout << "s2:   " << s2 << std::endl << "data: " << str2 << std::endl << std::endl;



        // 'Free' strPtr1 (Move it back to free list, to be reused)
        freeListAllocator->Free(strPtr1);

        // Reallocate a smaller memory size.
        // Address should be the same as first allocation as memory size fits.
        void* strPtr3 = freeListAllocator->Allocate(16, 8);
        std::cout << std::endl << "Reallocated memory address 3: @D " << strPtr3 << std::endl;

        strcpy(reinterpret_cast<char*>(strPtr3), s3.c_str());

        std::string str3{};
        for (int i = 0; i < s3.length(); i++)
        {
            std::size_t it = (std::size_t)strPtr3 + i;
            str3 += *(char*)it;
        }
        std::cout << "s3:   " << s3 << std::endl << "data: " << str3 << std::endl << std::endl;

        delete freeListAllocator;

        std::cout << "==============================================================" << std::endl;

        if (str3 == s3 && str2 == s2 && str1 == s1 && strPtr3 == strPtr1) std::cout << "TEST 3: PASS" << std::endl;
        else std::cout << "TEST 3: FAIL" << std::endl;

        std::cout << std::endl << std::endl << std::endl << std::endl;
    }


    // Test case 4 (Allocation, Writing, Freeing, Reallocation and Rewriting of memory)
    // 2 instances of memory allocation before first allocated memory data is freed.
    // Reallocated memory > first instance of memory allocation. Memory address CANNOT be reused as size DO NOT fit.
    // Therefore, reallocated memory address should NOT match first allocated memory address.
    void FreeListAllocator_Test4(std::string s1, std::string s2, std::string s3)
    {
        std::cout << "==============================================================" << std::endl;
        std::cout << "Free List Allocator @ Test 4" << std::endl;
        std::cout << "==============================================================" << std::endl;

        // Add a huge chunk of memory at start of application and initialise the allocator.
        Allocator* freeListAllocator = new FreeListAllocator((size_t)1e8, FreeListAllocator::Policy::FIND_FIRST);


        // First Instance


        // Allocating a small chunk of memory
        void* strPtr1 = freeListAllocator->Allocate(24, 8);
        std::cout << std::endl << "Allocated memory address 1: @D " << strPtr1 << std::endl;

        // Writing of string param to memory.
        strcpy(reinterpret_cast<char*>(strPtr1), s1.c_str());

        // Iterate through allocated memory and get its data.
        std::string str1{};
        for (int i = 0; i < s1.length(); i++)
        {
            std::size_t it = (std::size_t)strPtr1 + i;
            str1 += *(char*)it;
        }
        std::cout << "s1:   " << s1 << std::endl << "data: " << str1 << std::endl << std::endl;


        // Second Instance


        void* strPtr2 = freeListAllocator->Allocate(24, 8);
        std::cout << std::endl << "Allocated memory address 2: @D " << strPtr2 << std::endl;

        strcpy(reinterpret_cast<char*>(strPtr2), s2.c_str());

        // Iterate through allocated memory and get its data.
        std::string str2{};
        for (int i = 0; i < s2.length(); i++)
        {
            std::size_t it = (std::size_t)strPtr2 + i;
            str2 += *(char*)it;
        }
        std::cout << "s2:   " << s2 << std::endl << "data: " << str2 << std::endl << std::endl;



        // 'Free' strPtr1 (Move it back to free list, to be reused)
        freeListAllocator->Free(strPtr1);



        // Reallocate a smaller memory size.
        // Address should be the same as first allocation as memory size fits.
        void* strPtr3 = freeListAllocator->Allocate(64, 8);
        std::cout << std::endl << "Reallocated memory address 3: @D " << strPtr3 << std::endl;

        strcpy(reinterpret_cast<char*>(strPtr3), s3.c_str());

        std::string str3{};
        for (int i = 0; i < s3.length(); i++)
        {
            std::size_t it = (std::size_t)strPtr3 + i;
            str3 += *(char*)it;
        }
        std::cout << "s3:   " << s3 << std::endl << "data: " << str3 << std::endl << std::endl;

        delete freeListAllocator;

        std::cout << "==============================================================" << std::endl;

        if (str3 == s3 && str2 == s2 && str1 == s1 && strPtr3 != strPtr1) std::cout << "TEST 4: PASS" << std::endl;
        else std::cout << "TEST 4: FAIL" << std::endl;

        std::cout << std::endl << std::endl << std::endl << std::endl;
    }


    /******************************************************************************/
    /*!
        CustomAllocator Test cases function implementations. REQUIRES MANUAL CHECKS.
    */
    /******************************************************************************/

    // Call function to test whether Custom Allocator is working as intented.
    // Test cases for Custom Allocator.
    void CustomAllocator_Test()
    {
        CustomAllocator_Test1<TestComponent>();
    }

} // end of namespace