# **Custom Memory Management System** 
A tailored memory management system, offering custom allocators to optimize memory usage and performance. 
This system is built to handle dynamic memory allocation efficiently, minimizing fragmentation and provides 
resuable memory blocks for custom objects and components.

**Source Folders**: `include/Memory/`, `source/Memory/`

---

## Features
### 1. **Custom Allocators**:
- `FreeListAllocator`: Allocates a large memory block using a free list; supports `FIND_FIRST` and `FIND_BEST` allocation strategies.
- `PoolAllocator`: Divides memory into fixed-size chunks for efficient allocation.
- `CustomAllocator`: Templated allocator combining both above; manages specific object types and handles construction and destruction explicitly.

### 2. **Data Structures**:
- `SinglyLinkedList`: Used by `FreeListAllocator`; a simple linked list used for free block management.
- `StackLinkedList`: Used by `PoolAllocator`; a stack-based linked list used to manage chunk stacks.

### 3. **Unit Testing**:
- Comprehensive test cases to validate allocator behavior and ensure reliability.

---

## Architecture
Designed with modularity and extensibility, each allocator inherits from a base `Allocator` class, ensuring a unified interface for:
    - Initialization
    - Allocation
    - Deallocation
    - Memory Reset

### Key Components:
1. **FreeListAllocator**:
    - Allocates memory from a pre-allocated chunk.
    - Supports merging adjacent free blocks upon deallocation.
    - Offers two allocation strategies: `FIND_FIRST` and `FIND_BEST`.

2. **PoolAllocator**:
    - Divides memory into fixed-size chunks for efficient allocation.
    - Can use its own memory or memory provided by another allocator (`FreeListAllocator`).

3. **CustomAllocator**:
    - A templated allocator that combines `FreeListAllocator` and `PoolAllocator`.
    - Explicitly handles objects lifecycle (constructors, destructors).

4. **Data Structures**:
    - `SinglyLinkedList`: Tracks free blocks in the free list allocator.
    - `StackLinkedList`: Manages chunk stack in the pool allocator.

---

# **Entity Component System (ECS)**
The ECS is a modular and efficient architecture for managing entities, components, and systems in a game engine. It decouples data (components) from behavior (systems), enabling scalability, flexibility, and performance optimization across large number of game objects.

**Source Folders**: `include/ECS/`, `source/ECS/`, `include/Tag`, 'source/Tag'

---

## Features
### 1. **Core Architecture**:
- **Entity Management**:
  - Entities are lightweight identifiers that represent game objects.
  - Managed by the `EntityManager`, which handles creation, destruction, and ID recycling.

- **Component Management**:
  - Components store raw data and are tightly packed for performance.
  - Managed by the `ComponentManager`, which ensures efficient storage and retrieval.

- **System Management**:
  - Systems define behavior and operate on entities with specific component signatures.
  - Managed by the `SystemManager`, which updates systems based on entity signatures.

### 2. **Parent-Child Relationships**:
- Supports hierarchical relationships between entities.
- Enables complex object compositions by allowing child entities to be attached to parent entities.

### 3. **Tag and Layer Management**:
- Tags and layers allow categorization of entities for filtering and organization.
- Useful for grouping entities for specific behaviors (physics) or rendering.
- Backed by a bitset representation, where each bit corresponds to a layer, this allows fast computation and filtering.

### 4. **Extensibility**:
- Easily add new components and systems by registering them with the `Coordinator`.
- Systems automatically update their entity lists based on component signatures.

---

## Architecture
The ECS is composed of three core managers; `EntityManager`, `ComponentManager`, and `SystemManager`, and is coordinated through a central `Coordinator` class. This separation of concerns allows modular control and clean data flow across engine.

### Key Components:
1. **EntityManager**:
   - Handles entity creation, destruction, and ID management.
   - Uses recycled ID pool for efficient reuse.

2. **ComponentManager**:
   - Manages storage of components by type.
   - Ensures components are tightly packed for efficient iteration.
   - Maps entity-component associations.

3. **SystemManager**:
   - Manages systems and their associated entity lists.
   - Updates entity lists dynamically based on system-specific component signatures.

4. **Coordinator**:
   - Acts as a mediator between the `EntityManager`, `ComponentManager`, and `SystemManager`.
   - Provides a unified interface for creating entities, adding components, and managing systems.

---

## How It Works

1. **Entity Creation**:
   - Use the `Coordinator` to create entities.
   - Example:
     ```cpp
     EntityID entity = gCoordinator.CreateEntity();
     ```

2. **Component Assignment**:
   - Add components to entities using the `Coordinator`.
   - Example:
     ```cpp
     gCoordinator.AddComponent<Transform>(entity, glm::vec3(0.f, 0.f, 0.f));
     ```

3. **System Registration**:
   - Register systems and define their component signatures.
   - Example:
     ```cpp
     gCoordinator.RegisterSystem<TransformSystem>();
     gCoordinator.AssignSystemSignature<TransformSystem, Transform>();
     ```

4. **System Updates**:
   - Systems automatically update their entity lists based on component signatures.
   - Example:
     ```cpp
     auto transformSystem = gCoordinator.GetSystem<TransformSystem>();
     transformSystem->Update();
     ```

---

## Conclusion

The ECS architecture provides a robust and scalable foundation for game development. Its modular design ensures flexibility, while its efficient data management enables high performance. By decoupling data and behavior, the ECS simplifies the development of complex game systems.

---