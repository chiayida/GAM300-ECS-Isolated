# **Custom Memory Management System** 
A tailored memory management system, offering custom allocators to optimize memory usage and performance. 
This system is built to handle dynamic memory allocation efficiently, minimizing fragmentation and provides resuable memory blocks for custom objects and components.

**Source Folders**: `include/Memory/`, `source/Memory/`

---

## Features
### 1. **Core Architecture**:
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

## Conclusion
The Custom Memory Management System offers efficient and modular memory handling through specialized allocators tailored for different allocation patterns. 
Its design minimizes fragmentation and provides precise control over allocation, making it well-suited for performance-critical applications.

---

# **Entity Component System (ECS)**
The ECS is a modular and efficient architecture for managing entities, components, and systems in a game engine. 
It decouples data (components) from behavior (systems), enabling scalability, flexibility, and performance optimization across large number of game objects.

**Source Folders**: `include/ECS/`, `source/ECS/`, `include/Tag/`, `source/Tag/`

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
The ECS is composed of three core managers; `EntityManager`, `ComponentManager`, and `SystemManager`, and is coordinated through a central `Coordinator` class. 
This separation of concerns allows modular control and clean data flow across engine.

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

## Conclusion
The ECS architecture provides a robust and scalable foundation for game development. 
Its modular design ensures flexibility, while its efficient data management enables high performance. 
By decoupling data and behavior, the ECS simplifies the development of complex game systems.

---

# **Serialization**
The Serialization module provides a robust and extensible system for saving and loading data, including entities, components, and prefabs. 
It leverages the RTTR (Run-Time Type Reflection) library for reflection and the nlohmann/json library for JSON parsing and generation. 
This system ensures consistent persistence and restoration of runtime state, prefabs, and configuration data.

**Source Folders**: `include/Serialization/`, `source/Serialization/`

---

## Features
### 1. **Serialization**:
   - Converts entities, components and data structures into JSON format.
   - Supports primitive types, sequential containers (e.g., arrays, vectors), and associative containers (e.g., maps, sets).
   - Handles special cases like GLM types (`glm::vec3`, `glm::mat4`, etc.) for efficient serialization.
   - Enables saving of runtime state and configurations for reuse or analysis.

### 2. **Deserialization**:
   - Reads JSON files and reconstructs entities, components, and prefabs from JSON files.
   - Dynamically resolves types and structures of objects using RTTR.
   - Supports restoring hierarchical relationships between objects (parent-child relationships).

### 3. **Prefab Management**:
   - Allows saving and loading of prefabs, which are reusable templates for entities and components.
   - Supports prefabs updates, enabling changes to be applied across all instances.

---

## Architecture
The Serialization module is designed to be extensible and efficient, using reflection to dynamically handle various types of data. 
It integrates seamlessly with the ECS architecture, eliminating the need for manual serialization logic and simplifies the handling of various data types.

### Key Components:
1. **Serializer Class**:
   - Core interface for serialization and deserialization.
   - Provides static methods to handle different needs.

2. **RTTR Reflection**:
   - Enables inspection and manipulation of data at runtime.
   - Requires registration of serializable types and properties.

3. **JSON Integration**:
   - Uses nlohmann/json library for parsing and generating JSON files.
   - Supports custom formatting and error handling for reliable file operations.

---

## Conclusion
The Serialization module offers a structured and flexible approach to persisting and restoring runtime data. 
By leveraging reflection and JSON integration, it simplifies data management, suports reusable templates, and enables consistent state recovery across sessions.

---

# **Graphics** 

**Source Folders**: `include/Graphics/`, `source/Graphics/`

---