## Contents
- [ComponentPool](#componentpool)
- [ComponentCluster](#componentcluster)
- [ComponentReferenceList](#componentreferencelist)
- [ComponentManager](#componentmanager)

## <a id="componentpool"></a>ComponentPool
#### Defined in header `<ComponentPool.hpp>`
### Overview
This is a class that holds components and provides an interface to add, remove and access them.  
An instance of this class cannot be constructed outside ComponentCluster.
### Member functions
- `ComponentPool(std::pmr::memory_resorce* memres)`
  - Parameters:
  memres - a memory resource that will be used to initialize all of its private fields
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
  - Note: It wipes the entire memory of the ComponentPool.

### References
- [ComponentCluster](#componentcluster)
## <a id="componentcluster"></a>ComponentCluster
#### Defined in header `<ComponentCluster.hpp>`
### Overview
This is a structure that holds ComponentPool instances and provides an interface to synchronize them.  
It also provides a way to load and unload all components to and from ComponentManager.  
To add a new component you need to add a new public field of type `ComponentPool<desired component type>` and update the following methods whitch should
run the following functions for all ComponentPools:  
- `load(): AppData::getComponentManager().ComponentReferenceListName.push(&ComponentPoolName.m_data);`
- `unload(): AppData::getComponentManager().ComponentReferenceListName.pop(&ComponentPoolName.m_data);`
- `p_performeRemovals(): p_resolveRemovals();`
- `p_resolveDependencies(): p_resolveDependencies<dependency type (DT)>();`
- `p_performeResolvingCleanup(): .m_movedEnts.clear();`
- `p_performeAdditions(): p_resolveAdditions(); m_entitiesToAdd.clear();`  
Each scene should have exactly one instance of ComponentCluster and run ComponentCluster.load() ,after initializing and emplacing all components, and unload()
when the scene no longer needs to be updated.
### Member functions
- `void load()`
  - Note: It adds a reference to all ComponentPools data to corresponding ComponentReferenceLists in ComponentManager.
- `void unload()`
  - Note: It removes a reference from all ComponentPools data to corresponding ComponentReferenceLists in ComponentManager.
- `void syncComponentData()`
  - Note: It syncs a ComponentCluster.
### References
- [ComponentPool](#componentpool)
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
This class should only have exactly one instance inside AppData. It is used as "global" way to access all loaded components for systems.  
When adding a new component, a new CRL of a type of new component should be added as its public field;
### References
- [AppData](AppCore.md#appdata)
- [ComponentReferenceList](#componentreferencelist)
