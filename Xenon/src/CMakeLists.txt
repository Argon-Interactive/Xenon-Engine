set(XENON_SRC 
	api.h

	Xenon/entryPoint.hpp
	Xenon/application.cpp
	Xenon/application.hpp
	Xenon/scene.cpp
	Xenon/scene.hpp

	core/application.cpp
	core/application.hpp
	core/appData.cpp
	core/appData.hpp
	core/event.cpp
	core/event.hpp
	core/input/input.hpp
	core/input/input.cpp

	rendering/shader.cpp
	rendering/shader.hpp

	window/window.cpp
	window/window.hpp

	scenes/scene.cpp
	scenes/scene.hpp
	scenes/sceneManager.cpp
	scenes/sceneManager.hpp
	
	ECS/ComponentID.hpp
	ECS/ComponentManager.cpp
	ECS/ComponentManager.hpp
	ECS/ComponentsPool.hpp
	ECS/LinkedArray.hpp
	ECS/systemTest.cpp
	ECS/systemTest.hpp
)

add_library(XenonEngine SHARED ${XENON_SRC})

target_include_directories(XenonEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(XenonEngine PUBLIC XENON_DLL_BUILD)

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb adt)
