## Contents
- [ResourceManager](#resourcemanager)
- [Resource](#resource)
- [ResourceID](#resourceid)
- [ResourceType](#resourcetype)
- [ResourceFlag](#resourceflag)
- [ResourceHandle](#resourcehandle)
- [ResourceMetadata](#resourcemetadata)

## <a id="resourcemanager"></a>ResourceManager
#### Defined in header `<core/resourcesManagment/resourceManager.hpp>`
### Overview
This class should have only one instance inside the AppData. It is used to menage loading, unloading and retrieve data of the resource in a threadsave manner.
It takes a path to the file containing game data in its constructor. It opens the file, checks if it's valid and loads all the ResourceHandles and creates
ResourceMetadata for all resources.
### Member functions
- `void load(ResourceID[], size_t)` - Requestes that all the resources whitch ResourcesIDs were specified in an array given as an argument. It does not ensure
that all the resources will be loaded upon return from this function.
    - Overrides:
        - `void load(std::vector<ResourceID>&)`
        - `void load(ResourceID)`
- `void free(ResourceID[], size_t)` - It will free the resources whitch ResourceIDs were specifies in an array gives as an argument.
    - Overrides:
        - `void free(std::vector<ResourceID>&)`
        - `void free(ResourceID)`   


Note: Resources are "reference counted". They are loaded when for the first time requested and freed when they have been freed the same amount of times as 
requested. For safety each time you use a Resource you should load it and one you're done with it, it should be freed.
- `void sync()` - Stops the execution of the thread from which it was called from until all the requested resources are loaded.
- `std::filepath::path* getFilePath()` - Returns the filepath to the file with game data.
- `std::optional<Resource> getResource(ResourceID)` - It return the Resource with the ResourceID given as an argument if it is loaded and a nullopt if it isn't.
- `Resource getSyncedResource(ResourceID)` - It returns the Resource with the ResourceID given as an argument, if the Resource isn't loaded it stops the
execution of a thread it was called from until requested Resource is loaded.
### References
- [ResourceID](#resourceid)
- [Resource](#resource)
- [ResourceMetadata](#resourcemetadata)
- [ResourceHandle](#resourcehandle)
- [AppData](AppCore.md#appdata)

## <a id="resource"></a>Resource
#### Defined in header `<core/resourcesManagment/resource.hpp>`
### Overview
This struct stores resource data. `data` is an uint8_t* to the raw data of the resource and `size` is a size_t size of the resource in bytes.

## <a id="resourceid"></a>ResourceID
#### Defined in header `<core/resourcesManagment/resource.hpp>`
### Overview
This is an uint64_t used for addressing resources. Valid indieces range from [0; ResourceManager::m_metadatas.size()].
### References
[ResourceManager](#resourcemanager)

## <a id="resourcetype"></a>ResourceType
#### Defined in header `<core/resourcesManagment/resource.hpp>`
### Overview
This is an enum : uint32_t that stores the type of the resource.

## <a id="resourceflag"></a>ResourceFlag
#### Defined in header `<core/resourcesManagment/resource.hpp>`
### Overview
This is a flag of 32 bits that stores the state of the resource.  
Bit meaning:
Index - Name - Explanation
0 - Valid - Whether the data in a state that is usable be anything that would request this data
1 - Compressed - Whether the data is compressed
2 - Encrypted - Whether the data is encrypted
### Member functions
- `void setFlag(uint32_t)` - Sets the flag to the uint32_t passed as an argument.
- `bool isValid()` - Returns the state of the valid flag. This flag is set to 1 if compressed and encrypted flags are 0, 0 otherwise.
- `bool isCompressed()` - Returns the state of the compressed flag.
- `bool isEncrypted()` - Returns the state of the encrypted flag.
- `void setCompressed()` - Sets the compressed flag to 1.
- `void setEncrypted()` - Sets the encrypted flag to 1.
- `void resetCompressed()` - Sets the compressed flag to 0.
- `void resetEncrypted()` - Sets the encrypted flag to 0.

## <a id="resourcehandle"></a>ResourceHandle
#### Defined in header `<core/resourcesManagment/resource.hpp>`
### Overview
This is a struct that is used to store information about the position of the resource inside a file, its size in bytes and its state as an uint32_t flag that is
later passed to a ResourceFlag, and a ResourceType.
### References
- [ResourceType](#resourcetype)
- [ResourceFlag](#resourceflag)

## <a id="resourcemetadata"></a>ResourceMetadata
#### Defined in header `<core/resourcesManagment/resource.hpp>`
### Overview
This class is used to store and manage a resource. It is resposible for loading and freeing data in an asynchronous way.
### Member functions
- `void changePMR(std::pmr::memory_resource*)` - It changes the memory resource that is used to store the resource data. When the PMR changes the old memory 
will be cleaned and moved into the new PMR.
- `std::optional<std::future<void>> load(ResourceHandle, std::function<std::vector<uint8_t>&>)` - This function is used to ensure that a resource is loaded into
memory. It keeps a count of how many times the resource was requested. The resource is asynchronously loaded into memory the first time it is requested and a 
future of this asynchronous operation is returned, otherwise std::nullopt.
`void unload()` - This function decrements one from requests count, if it reaches 0 the resource if freed form memory.
`uint8_t* getRawData()` - It returns a pointer to the data, however it does not ensure that the data is loaded.
`size_t getSize()` - It returns the size of the data in bytes. 0 if the data is not loaded.
`ResourceFlag* getFlag()` - It returns the pointer to the flag, however it does not ensure that the data is loaded.
### References
- [ResourceFlag](#resourceflag)
- [ResourceHandle](#resourcehandle)
