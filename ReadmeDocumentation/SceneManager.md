## Contents
- [Scene](#scene)
- [Scene Manager](#scenemanager)

## <a id="scene"></a>Scene
#### Defined in header `<scenes/scene.hpp>`
### Overview
This class represents a game scene which holds it's components via ComponentCluster and provides an interface for creating, deleting and managing entities and their components at runtime.
### Member functions
- `Scene()`
  - Return value: an empty, runtime-created scene
- `Scene(uint64_t buildIndex)`
  - Parameters:
    - buildIndex - the index of scene to load from ResourceManager
  - Return value: a scene of provided build index
  - Note: this constructor is meant to create scene instance from raw scene data stored in build file
- `unload()`
  - 

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
- [Scene]( )
- [AppData]( )
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
- [AppData]( )
- [ComponentReferenceList](#componentreferencelist)
