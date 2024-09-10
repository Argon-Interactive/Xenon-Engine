## Contents
- [Component](#component)
- [ComponentPool](#componentpool)
- [ComponentCluster](#componentcluster)
- [ComponentReferenceList](#componentreferencelist)
- [ComponentManager](#componentmanager)
- [ChunkedArray](#chunkedarray)
- [Reference](#reference)
- [ComponentContainerTuple](#componentcontainertuple)
- [ContainerTuple](#containertuple)

## <a id="component"></a>Component
#### Defined in header `<ECS/component.hpp>`
### Overview
Templated component base class which every component must properly derive from.
It stores the ID of the owner of an entity which is used by the ComponentPool as well as provides default move/copy semantics to ensure Reference validity.
The proper derivation of this class should be done using curiously recurring template pattern (CRTP) as follows:
```
struct MyComponent : Component<MyComponent> { ... }
```
which can be simplified with COMPONENT declaration macro:
```
COMPONENT(MyComponent) { ... }
```
In order for the component to be safely usable it is unadvised to implicitly provide destructor as well as copy/move constructor/assign operator.
If it is necessary, remember: copy/move constructor/assign operators should invoke its base class counterparts.
### Member functions
- 'void p_atDelete() private'
  - Description: the only relevant method - it should be called by ComponentPool at destruction.
### Non-member functions
- `bool is_proper_component<T>()`
  - Return value: (at compile time) returns whether T satisfies all component requirements.
### References
- [ComponentPool](#componentpool)
- [Reference](#reference)

## <a id="componentpool"></a>ComponentPool
#### Defined in header `<ECS/componentPool.hpp>`
### Overview
This is a templated class that holds components and provides an interface to add, remove and access them.  
Template parameter T has to be a proper component implementation:
1) must be derived from Component<T> template (it can be done via COMPONENT() declaration macro)
2) must be default constructible and copy/move constructible/assignable.
### Member functions
- `ComponentPool(std::pmr::memory_resorce* memres)`
  - Parameters:
    - memres - a memory resource that will be used to initialize underlying ChunkedArray container.
- `T& hasComponent(Entity ent)`
  - Parameters:  
    - ent - Entity to check if it has component of this type.
- `T& getComponent(Entity ent)`
  - Parameters:  
    - ent - Entity that owns required component.
  - Return value: reference to this entity's component T.
- `T* getComponentPtr(Entity ent)`
  - Parameters:  
    - ent - Entity that owns required component.
  - Return value: pointer to this entity's component T.
- `T* emplaceComponent(Entity ent, Args ...args)`
  - Parameters:  
    - ent - Entity that will own the new component.
    - args - constructor arguments of the component.
  - Return value: returns a pointer to created component.
- `void addComponent(Entity ent)`
  - Parameters:  
    - ent - Entity that will own the new component.
- `void addComponent(Entity ent, const T& data)`
  - Parameters:  
    - ent - Entity that will own the new component.
    - data - The component that will be taken copy of.
- `void addComponent(Entity ent, T&& data)`
  - Parameters: 
    - ent - Entity that will own the new component.
    - data - Component that will be moved to the entity.
- `void removeComponent(Entity ent)`
  - Parameters:  
    - ent - Entity whose component will be removed.
- `void purge()`
  - Description: It wipes the entire memory of the ComponentPool.
### References
- [Component](#component)
- [ComponentCluster](#componentcluster)
- [ChunkedArray](#chunkedarray)

## <a id="componentcluster"></a>ComponentCluster
#### Defined in header `<ECS/componentCluster.hpp>`
### Overview
This is a structure that holds ComponentPool instances and via ContainerTuple and serves as an access point to components through a scene.
It also provides a way to load and unload all components to and from ComponentManager. 
The complete list of components is taken from ComponentContainerTuple type.
### Member functions
- `void load()`
  - Description: It adds a reference to all ComponentPools data to corresponding ComponentReferenceLists in ComponentManager.
- `void unload()`
  - Description: It removes a reference from all ComponentPools data to corresponding ComponentReferenceLists in ComponentManager.
- `ComponentPool<T>& get<T>()`
  - Return value: the ComponentPool of corresponding component type T.
### References
- [Component](#component)
- [ComponentPool](#componentpool)
- [ComponentContainerTuple](#componentcontainertuple)
- [ComponentReferenceList](#componentreferencelist)
- [ComponentManager](#componentmanager)
- [Scene](SceneManagement.md#scene)
- [AppData](AppCore.md#appdata)

## <a id="componentreferencelist"></a>ComponentReferenceList
#### Maybe referred to as CRL
#### Defined in header `<ECS/componentReferenceList.hpp>`
### Overview
It stores references to loaded components from all scenes and provides an interface for iterating over them.
### Member functions
- `ComponentReferenceList(std::pmr::memory_resorce* memRsrc)`
  - Parameters:
    - memRsrc - the memory resource the reference list will use for its allocations.
- `bool contains(ChunkedArray<T>* elem)`
  - Parameters:
    - elem - target chunked array instance.
  - Return value: true if the elements of the array are included in reference list.
- `iterator begin()`
  - Return value: an iterator to the begining of the list.
- `iterator end()`
  - Return value: an iterator to the end of the list.
- `void push(ChunkedArray<T>* elem) private`
  - Parameters:  
    - elem - a pointer to a ChunkedArray<T> that will be added.
- `void pop(ChunkedArray<T>* elem) private`
  - Parameters:  
    - elem - a pointer to a ChunkedArray<T> that will be removed.
### References
- [Component](#component)
- [Scene](SceneManagement.md#scene)
- [ChunkedArray](#chunkedarray)

## <a id="componentmanager"></a>ComponentManager
#### Defined in header `<ECS/componentManager.hpp>`
### Overview
This class is used as a global way (through AppData) to access reference lists of components for systems.  
It takes the list of all Component types from ComponentContainerTuple.
### Member functions
- `ComponentReferenceList<T>& get<T>()`
  - Return value: the ComponentReferenceList of desired component type T.
### References
- [AppData](AppCore.md#appdata)
- [ComponentReferenceList](#componentreferencelist)
- [ComponentContainerTuple](#componentcontainertuple)

## <a id="chunkedarray"></a>ChunkedArray
#### Defined in header `<ECS/chunkedArray.hpp>`
### Overview
Templated container that is intended to avoid realocations entirely. Stores data in 4kB chunks which are being added/deleted as needed.
### Member functions
- `ChunkedArray(std::pmr::memory_resorce* memoryRsrc)`
  - Parameters:
    - memoryRsrc - the resource used to allocate arrays chunks.
- `iterator begin()`
  - Return value: an iterator to the begining of the array.
- `iterator end()`
  - Return value: an iterator to the end of the array.
- `T* emplace_back(Args&&... args)`
  - Parameters:
    - ...args - arguments that are passed to the constructor of T.
  - Return value: pointer to newly emplaced element.
  - Note: the new element is placed at the end of the array.
- `void push_back(const T& value)`
  - Parameters:
    - value - object copied to the end of the array.
- `void push_back(T&& value)`
  - Parameters:
    - value - object moved to the end of the array.
- `void pop_back()`
  - Description: pops the last element from the array.
- `void clear()`
  - Description: clears contents of the array.

## <a id="reference"></a>Reference
#### Defined in header `<ECS/reference.hpp>`
### Overview
Templated class of reference/pointer type that is designed to stay valid even after the referenced object is moved.
The reference has to be a component type.
For referencing game assets use (NOT IMPLEMENTED YET).
### Member functions
- `T& get()`
  - Return value: a reference to the referenced component (hell yeah that's fuckin' useful info right there).
  - Note: this method might try do dereference nullptr if the reference got destroyed, reset or was simply not set.
- `T* getPtr()`
  - Return value: a pointer to the referenced component.
- `void set(T* dep)`
  - Parameters:
    - dep - pointer to component to set the reference to
- `void set(T& dep)`
  - Parameters:
    - dep - component to set the reference to
### References
- [Component](#component)

## <a id="componentcontainertuple"></a>ComponentContainerTuple
#### Defined in header `<ECS/componentContainerTuple.hpp>`
### Overview
It is just a template `using` declaration for ContainerTuple types with a list of all component types.
To add a new component type add it's type name to the list.
### Usage
To define a tuple of Containers with all component types just use:
```ComponentContainerTuple<Container> tupleOfContainers;```
e.g.
```ComponentContainerTuple<std::vector> vectors;```
will produce a tuple like:
```
struct {
std::vector<Transform> v0; 
std::vector<Model3D> v1; 
std::vector<RigidBody> v2; 
...
} vectors;
```
Note that the order of elements in tuples depends solely on the order in the list declaration (which is to say every component container tuple will have the same order of elements).
### References
- [Component](#component)
- [ContainerTuple](#containertuple)

## <a id="containertuple"></a>ContainerTuple
#### Defined in header `<ECS/containerTuple.hpp>`
### Overview
Tuple-like structure to hold a list of containers with different stored types.
### Template arguments
- Container - a temlate argument which will be used as a container type
  - Note: it has to be of type `template<typename T> typename` which is to say it has to take one typename (stored type) as its template argument
- ...Types - variadic list of types stored in container list
### Example
Idk I feel like an example might help:
`ContainerTuple<std::vector, int, float, char, std::string>` is equivalent to a struct:
```
struct {
  std::vector<int> m0;
  std::vector<float> m1;
  std::vector<char> m2;
  std::vector<std::string> m3;
}
```
### Member functions
- `ContainerTuple()`
  - Note: uses default initialization for all its containers.
- `ContainerTuple(auto arg)`
  - Parameters:
    - arg - an argument passed to each container constructor during initialization.
- `ContainerTuple(auto... args)`
  - Parameters:
    - ...args - list of arguments passed individually to containers.
- `auto& get<N>()`
  - Template parameters:
    - N - index of desired container.
  - Return value: returns the container at index N.
  - Note: the order of containers is exactly the same as in the template declaration.
  - Note: this method is constexpr meaning it will simply be evaluated to desired container reference at runtime.
- `auto& get<U>()`
  - Template parameters:
    - U - type of stored data in container.
  - Return value: returns the container storing U type elements.
  - Note: while ContainerTuple allows for type duplicates, this method will only return the left-most container (with lowest index).
  - Note: this method is constexpr meaning it will simply be evaluated to desired container reference at runtime.
  - Note: will generate static_assertion error if there is no container with type U in the tuple.
### Member fields
- `size`
  - Description: static constexpr size of the tuple
### Non-member functions
- `for_each_indexed(Func&& func, Tuples&... tuples)`
  - Parameters
    - func - function to iterate with. Has to take `std::size_t i, Tuples&... tuples` as its arguments
    - ...tuples - the tuples to be iterated
  - Description: Serves as a `for(int i = 0; i < min(tuples::size...); i++)` but constexpr.
    It is expanded to "manual-like" iteration:
    ```
    for_each_indexed([](auto& vec, auto& list){
        std::cout << i << " " << vec.max_size() << " " << list.front() << "\n";
    }, vectors, lists);
    ```
    is equivalent to:
    ```
    std::cout << 0 << " " << vectors.get<0>().max_size() << " " << lists.get<0>().front() << "\n";
    std::cout << 1 << " " << vectors.get<1>().max_size() << " " << lists.get<1>().front() << "\n";
    std::cout << 2 << " " << vectors.get<2>().max_size() << " " << lists.get<2>().front() << "\n";
    std::cout << 3 << " " << vectors.get<3>().max_size() << " " << lists.get<3>().front() << "\n";
    ...
    ```
- `for_each(Func&& func, Tuples&... tuples)`
  - Parameters
    - func - function to iterate with. Has to take `Tuples&... tuples` as its arguments
    - ...tuples - the tuples to be iterated
  - Note: it is basically `for_each_indexed` but the function does not use the index
  - Description: Serves as a `for(int i = 0; i < min(tuples::size...); i++)` but constexpr.
    It is expanded to "manual-like" iteration:
    ```
    for_each_indexed([](auto& vec, auto& list){
        std::cout << vec.max_size() << " " << list.front() << "\n";
    }, vectors, lists);
    ```
    is equivalent to:
    ```
    std::cout << vectors.get<0>().max_size() << " " << lists.get<0>().front() << "\n";
    std::cout << vectors.get<1>().max_size() << " " << lists.get<1>().front() << "\n";
    std::cout << vectors.get<2>().max_size() << " " << lists.get<2>().front() << "\n";
    std::cout << vectors.get<3>().max_size() << " " << lists.get<3>().front() << "\n";
    ...
    ```
