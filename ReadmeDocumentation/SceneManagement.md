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
- `createEntity()`
  - Return value: ID of newly created entity
  - Note: IDs of entities are global which means every two entities even from different scenes are guaranteed to have different IDs
  - Note: the method is thread safe
- `deleteEntity(Entity entity)`
  - Parameters:
    - entity - entity ID to delete
- `bool hasComponent<T>(Entity entity)`
  - Parameters:  
    - entity - Entity to check if it has component T.
  - Return value: return whether or not the entity has component T.
- `T& getComponent<T>(Entity entity)`
  - Parameters:  
    - entity - Entity that owns required component of type T.
  - Return value: reference to this entity's component T.
- `void addComponent<T>(Entity ent)`
  - Parameters:  
    - ent - Entity that will be added component T.
- `void addComponent<T>(Entity entity, const T& data)`
  - Parameters:  
    - entity - Entity that will own the new component.
    - data - The component T that will be taken copy of.
- `void addComponent<T>(Entity entity, T&& data)`
  - Parameters: 
    - entity - Entity that will own the new component.
    - data - Component T that will be moved to the entity.
- `void removeComponent<T>(Entity ent)`
  - Parameters:  
    - ent - Entity whose component T will be removed.
- `getBuildIndex()`
  - Return value: build index of this scene
  - Note: if the scene was runtime-created this method returns 0 which is an invalid buildIndex
- `isRuntimeCreated()`
  - Return value: returns true if the scene has been created at runtime (with constructor Scene())
- `setEntityStartID(Entity id)`
  - Parameters:
    - id - upper bound for runtime-created entities 
  - Note: this value should be less than the lowest entity ID from any of the built scenes
  - Note: this method is meant to be only called once by AppData at the application launch
### References
- [ComponentCluster](ECS.md#componentcluster)
- [ResourceManager](ResourceManagement.md#resourcemanager)
- [AppData](AppCore.md#appdata)

## <a id="scenemanager"></a>SceneManager
#### Defined in header `<scene/sceneManager.hpp>`
### Overview
Management class for scenes meant to be accessed from AppData. Provides interface for loading and unloading scenes, as well as moving entites between them.
Allows for multiple scenes to be loaded and active at the same time. Loaded scenes are stored in std::vector.
Every method in this class is thread safe.
### Member functions
- `createScene()`
  - Return value: pointer to new, runtime-created scene
- `purge()`
  - Description: unloads every currently loaded scene
  - Note: before the purge, SceneManager waits for all async lodaing/unloading tasks to finish
- `purgeAsync()`
  - Description: unloads every currently loaded scene asynchronously
  - Note: before the purge, SceneManager waits for all async lodaing/unloading tasks to finish
- `getScene(uint64_t index)`
  - Parameters:
    - index - desired scene index in loaded scenes vector
  - Return value: pointer to scene stored at the provided index in loaded scenes vector
- `getSceneByBuildIndex(uint64_t buildIndex)`
  - Parameters:
    - buildIndex - desired scene build index
  - Return value: pointer to scene with a given build index
  - Note: in case more than one scene with a given build index is loaded, the one with lowest index is returned
- `getSceneCount()`
  - Return value: the number of loaded scenes
- `loadScene(uint64_t buildIndex)`
  - Parameters:
    - buildIndex - desired scene build index to be loaded
- `loadSceneAsync(uint64_t buildIndex)`
  - Parameters:
    - buildIndex - desired scene build index to be loaded asynchronously
- `unloadSceneAt(uint64_t index)`
  - Parameters:
    - index - index of scene to be unloaded in loaded scenes vector
- `unloadSceneAsyncAt(uint64_t index)`
  - Parameters:
    - index - index of scene to be unloaded asynchronously in loaded scenes vector
- `unloadScene(Scene* scene)`
  - Parameters:
    - scene - scene pointer to be unloaded
- `unloadSceneAsyncAt(Scene* scene)`
  - Parameters:
    - scene - scene pointer to be unloaded asynchronously
- `unloadScene(uint64_t buildIndex)`
  - Parameters:
    - buildIndex - build index of scene to be unloaded
  - Note: in case more than one scene with a given build index is loaded, the one with lowest index is unloaded
- `unloadSceneAsync(uint64_t buildIndex)`
  - Parameters:
    - buildIndex - build index of scene to be unloaded asynchronously
  - Note: in case more than one scene with a given build index is loaded, the one with lowest index is unloaded
- `switchScene(uint64_t buildIndex)`
  - Parameters:
    - buildIndex - build index of scene to be switched to
  - Note: the method first unloads all loaded scenes before loading the desired one all synchronously
- `switchSceneImmediate(uint64_t buildIndex)`
  - Parameters:
    - buildIndex - build index of scene to be switched to
  - Note: the method unloads all loaded scenes asynchronously then load the desired scene synchronously
- `switchSceneAsync(uint64_t buildIndex)`
  - Parameters:
    - buildIndex - build index of scene to be switched to
  - Note: the method first loads the desired scene asynchronously then unloads all previous ones asynchronously
- `setActiveSceneAt(uint64_t index)`
  - Parameters:
    - index - index of scene in loaded scenes vector to be set as active 
- `setActiveScene(Scene* scene)`
  - Parameters:
    - scene - pointer to scene to be set as active 
- `setActiveScene(uint64_t buildIndex)`
  - Parameters:
    - buildIndex - build index of a scene to be set as active 
  - Note: in case more than one scene with a given build index is loaded, the one with lowest index is set as active
- `getActiveScene()`
  - Return value: pointer to the current active scene
- `getActiveSceneIndex()`
  - Return value: index in loaded scenes vector of the current active scene
- `moveEntity(Entity entity, uint64_t targetSceneIndex)`
  - Parameters:
    - entity - entity to be moved
    - targetSceneIndex - index in loaded scenes vector of the scene to move the entity to
- `moveEntity(Entity entity, uint64_t targetSceneIndex)`
  - Parameters:
    - entity - entity to be moved
    - targetScene - pointer to the scene to move the entity to
### References
- [Scene](#scene)
- [AppData](AppCore.md#appdata)
