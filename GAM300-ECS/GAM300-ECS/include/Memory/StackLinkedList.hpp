/******************************************************************************/
/*!
\file       StackLinkedList.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       11 July 2022
\brief
  This file contains the declaration and implementation of
  Templated StackLinkedList class. It acts as a container for both
  CustomAllocator and PoolAllocator classes which is a stack + linked list.
*/
/******************************************************************************/
#pragma once

template <typename T>
class StackLinkedList 
{
public:
    struct Node 
    {
        T data;
        Node* nextNode;
    };

    StackLinkedList();
    StackLinkedList(StackLinkedList & stackLinkedList) = delete;

    void push(Node* newNode);
    Node* pop();

    Node* headNode;
};

template <typename T>
StackLinkedList<T>::StackLinkedList() : headNode{ nullptr } {}

template <typename T>
void StackLinkedList<T>::push(Node* newNode) 
{
    // New node to be at the top of stack.
    newNode->nextNode = headNode;
    headNode = newNode;
}

template <typename T>
typename StackLinkedList<T>::Node* StackLinkedList<T>::pop()
{
    Node* tempNode = headNode;
    headNode = headNode->nextNode;

    return tempNode;
}