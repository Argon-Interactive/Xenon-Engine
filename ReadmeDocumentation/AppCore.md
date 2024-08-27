## Contents
- [Application](#application)
- [Scene Manager](#scenemanager)

## <a id="application"></a>Application
#### Defined in header `<core/application.hpp>`
### Overview
This class is designed to manage the lifecycle an application.
It is responsible for handling main application loop, events and exceptions.
### Member functions
- `Application(const Xenon::AppConfig& config)`
  - Parameters:
    - config - passed down to AppData initialization. See Xenon::AppConfig.
- `pushEvent(const Core::Event& event)`
  - Parameters:
    - event - event instance to be dealt with by the application.
  - Note: this method is thread-safe.
  - Note: the events are dispatched synchronously by the main loop.
- `run()`
  - Return value: 0 if no errors occured. Other values if there was an exception in the main loop.
  - Note: this is the main loop of the application.
### References
- [AppData](#appdata)
- [Event](#event)
- [AppConfig]( )

## <a id="appdata"></a>AppData
#### Defined in header `<core/appData.hpp>`
### Overview
Static class working as a global access point for all the engine systems.
### Member functions
- `init(std::function<void(const Event&)> eventDispatcher, const Xenon::AppConfig& config)`
  - Parameters:
    - eventDispatcher - function used for all event callers in the application.
    - config - application config containg basic app information.
- `terminate()`
  - Description: shutdowns all engine systems.
- `get{system_name}()`
  - Return value: reference to a system.
### References
- [Event](#event)
- [AppConfig]( )
- [Window]( )
- [SceneManager](SceneManagement.md#scenemanager)
- [ComponentManager](ECS.md#componentmanager)
- [ResourceManager]( )

## <a id="event"></a>Event
#### Defined in header `<core/application.hpp>`
### Overview
This class represent basic events happening during application lifetime.
I really don't know more else is there to say. It just stores information.
