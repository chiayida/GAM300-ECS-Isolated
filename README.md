# **Custom Memory Management System** 
Files can be found in folders: include/Memory/, source/Memory/

---

A tailored memory management system, offering custom allocators to optimize memory usage and performance. 
This system is built to handle dynamic memory allocation efficiently, minimizing fragmentation and provides 
resuable memory blocks for custom objects and components.

---

## Features
### 1. **Custom Allocators**:
- `FreeListAllocator`: Allocates a large memory block using a free list; supports FIND_FIRST and FIND_BEST allocation strategies.
- `PoolAllocator`: Divides memory into fixed-size chunks for efficient allocation.
- `CustomAllocator`: Templated allocator combining both above; manages specific class types and handles object construction and destruction.

### 2. **Data Structures**:
- `SinglyLinkedList`: Used by `FreeListAllocator`; a simple linked list used for free block management.
- `StackLinkedList`: Used by `PoolAllocator`; a stack-based linked list used to manage chunk stacks.

### 3. **Unit Testing**:
- Includes test cases to validate the functionality of the allocators.

---

## Architecture
Designed with modularity and extensibility, each allocator inherits from a base `Allocator` class, ensuring a unified interface for:
- Initialization
- Allocation
- Deallocation (Freeing)
- Resetting memory

### Key Components:
1. **FreeListAllocator**:
- Allocates memory from a pre-allocated chunk.
- Merges adjacent free blocks upon deallocation.
- Supports two allocation policies: `FIND_FIRST` and `FIND_BEST`.

2. **PoolAllocator**:
- Divides memory into fixed-size chunks for efficient allocation.
- Can use its own memory or memory provided by another allocator (`FreeListAllocator`).

3. **CustomAllocator**:
- A templated allocator that combines `FreeListAllocator` and `PoolAllocator`.
- Explicitly handles allocated objects lifecycle (constructors, destructors).

4. **Data Structures**:
- `SinglyLinkedList`: Manages free memory blocks in `FreeListAllocator`.
- `StackLinkedList`: Manages memory chunk stack in `PoolAllocator`.

---