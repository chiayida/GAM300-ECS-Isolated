/******************************************************************************/
/*!
\file       SinglyLinkedList.hpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 300
\date       11 July 2022
\brief
  This file contains the declaration and implementation of
  Templated SinglyLinkedList class. It acts as a container for FreeListAllocator
  class which is a linked list that only goes in 1 direction.
*/
/******************************************************************************/
#pragma once

template <class T>
class SinglyLinkedList 
{
public:
    struct Node 
    {
        T data;
        Node* nextNode;
    };

    SinglyLinkedList();

    void insert(Node* prevNode, Node* newNode);
    void remove(Node* prevNode, Node* removeNode);

    Node* headNode;
};

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : headNode{ nullptr } {}

template <class T>
void SinglyLinkedList<T>::insert(Node* prevNode, Node* newNode) 
{
    // newNode will be the first node, newNode is headNode.
    if (prevNode == nullptr) 
    {
        // Linked list is not empty, make newNode's nextNode be the previous headNode.
        if (headNode != nullptr) 
        {
            newNode->nextNode = headNode;
        }
        // Linked list is empty.
        else 
        {
            newNode->nextNode = nullptr;
        }
        headNode = newNode;
    }
    // newNode will be the last node or will be in between 2 nodes.
    else 
    {
        // newNode is the last node
        if (prevNode->nextNode == nullptr) 
        {
            prevNode->nextNode = newNode;
            newNode->nextNode = nullptr;
        }
        // newNode is in between 2 nodes.
        else 
        {
            newNode->nextNode = prevNode->nextNode;
            prevNode->nextNode = newNode;
        }
    }
}

template <class T>
void SinglyLinkedList<T>::remove(Node* prevNode, Node* removeNode) 
{
    // removeNode is the first node of the linked list.
    if (prevNode == nullptr) 
    {
        // Linked list only contains 1 node.
        if (removeNode->nextNode == nullptr) 
        {
            headNode = nullptr;
        }
        // Linked list contains more than 1 node.
        else 
        {
            headNode = removeNode->nextNode;
        }
    }
    // removeNode is the lastNode or in between 2 nodes.
    else 
    {
        prevNode->nextNode = removeNode->nextNode;
    }
}