## Contents
- [ComponentPool](#componentpool)
- [ComponentCluster](#componentcluster)
- [ComponentReferenceList](#componentreferencelist)
- [ComponentManager](#componentmanager)
- [ComponentContainerTuple](#componentcontainertuple)
- [ContainerTuple](#containertuple) (probably shouldn't be here)

## <a id="componentpool"></a>ComponentPool
#### Defined in header `<ComponentPool.hpp>`
### Overview
This is a class that holds components and provides an interface to add, remove and access them.  
An instance of this class cannot be constructed outside ComponentCluster.
### Member functions
- `ComponentPool(std::pmr::memory_resorce* memres)`
  - Parameters:
    - memres - a memory resource that will be used to initialize all of its private fields
- `T& getComponent(Entity ent)`
  - Parameters:  
    - ent - Entity that owns required component
- `T* getComponentPtr(Entity ent)`
  - Parameters:  
    - ent - Entity that owns required component
- `T* emplaceComponent(Entity ent, Args ...args)`
  - Parameters:  
    - ent - Entity that will own the new component  
    - args - constructor arguments of T
  - Return value: returns a pointer to created component
  - Note: This funcion is not SRTS(Systems Run Time Save) so should be only used to add components before loading ComponentCluster.
- `void addComponent(Entity ent, const T& data)`
  - Parameters:  
    - ent - Entity that will own the new component  
    - data - The new component
  - Note: This is much slower then emplaceCompoennt but is SRTS. It will only take effect when a ComponentCluster is synced.
- `void removeComponent(Entity ent)`
  - Parameters:  
    - ent - Entity which component will be removed
  - Note: This will only take effect when a ComponentCluster is synced.
- `void purge()`
  - Description: It wipes the entire memory of the ComponentPool.

### References
- [ComponentCluster](#componentcluster)
## <a id="componentcluster"></a>ComponentCluster
#### Defined in header `<ComponentCluster.hpp>`
### Overview
This is a structure that holds ComponentPool instances and provides an interface to synchronize them.  
It also provides a way to load and unload all components to and from ComponentManager.  
The complete list of components is taken from ComponentContainerTuple type.
Resolving dependencies is currently fucked but we're working on it
Each scene should have exactly one instance of ComponentCluster and run ComponentCluster.load() ,after initializing and emplacing all components, and unload()
when the scene no longer needs to be updated.
### Member functions
- `void load()`
  - Description: It adds a reference to all ComponentPools data to corresponding ComponentReferenceLists in ComponentManager.
- `void unload()`
  - Description: It removes a reference from all ComponentPools data to corresponding ComponentReferenceLists in ComponentManager.
- `void syncComponentData()`
  - Description: It syncs a ComponentCluster.
- `ComponentPool<T>& get<T>()`
  - Return value: the ComponentPool of corresponding component type T.
### References
- [ComponentPool](#componentpool)
- [ComponentContainerTuple](#componentcontainertuple)
- [ComponentReferenceList](#componentreferencelist)
- [ComponentManager](#componentmanager)
- [Scene](SceneManagement.md#scene)
- [AppData](AppCore.md#appdata)
## <a id="componentreferencelist"></a>ComponentReferenceList
#### Maybe referred to as CRL
#### Defined in header `<ComponentReferenceList.hpp>`
### Overview
It stores references to loaded components data and provides an interface for iterating over them, using the standard iterator syntax `for(auto& a : CRL)` it 
will iterate over all loaded components and not the nodes of the underlying data structure, that is linked list.
### Member functions
- `void push(ChunkedArray<T>* elem) private`
  - Parameters:  
    - elem - a pointer to a ChunkedArray<T> that will be added.
- `void pop(ChunkedArray<T>* elem) private`
  - Parameters:  
    - elem - a pointer to a ChunkedArray<T> that will be removed.
### References
- [ChunkedArray]( )

## <a id="componentmanager"></a>ComponentManager
#### Defined in header `<ComponentManager.hpp>`
### Overview
This class should only have exactly one instance inside AppData. It is used as global way to access all loaded components for systems.  
It takes the list of all Component types from ComponentContainerTuple.
### Member functions
- `ComponentReferenceList<T>& get<T>()`
  - Return value: the ComponentReferenceList of desired component type T.
### References
- [AppData](AppCore.md#appdata)
- [ComponentReferenceList](#componentreferencelist)
- [ComponentContainerTuple](#componentcontainertuple)

## <a id="componentcontainertuple"></a>ComponentContainerTuple
#### Defined in header `<componentContainerTuple.hpp>`
### Overview
It is just a template `using` declaration for ContainerTuple types with all component types.
To add a new component type add it's type name to the list.
### Usage
To define a tuple of Containers with all component types just use:
```ComponentContainerTuple<Container> tupleOfContainers;```
e.g.
```ComponentContainerTuple<std::vector> vectors;```
### References
- [ContainerTuple](#containertuple)

## <a id="containertuple"></a>ContainerTuple
#### Defined in header `<containerTuple.hpp>`
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
