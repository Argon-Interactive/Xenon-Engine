set(XENON_SRC 
	Xenon/api.h

	Xenon/entryPoint.hpp
	Xenon/application.cpp
	Xenon/application.hpp
	Xenon/scene.cpp
	Xenon/scene.hpp
	Xenon/entity.cpp
	Xenon/entity.hpp

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
	
	ECS/entity.hpp
	ECS/componentManager.cpp
	ECS/componentManager.hpp
	ECS/componentCluster.hpp
	ECS/componentCluster.cpp
	ECS/componentPool.hpp
	ECS/componentReferenceList.hpp
	ECS/chunkedArray.hpp
	ECS/systemImplementations/system.hpp

	memory/debugMemoryResource.cpp
	memory/debugMemoryResource.hpp

	TEST/test.hpp
	TEST/test.cpp
)

add_library(XenonEngine SHARED ${XENON_SRC})

target_include_directories(XenonEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(XenonEngine PUBLIC XENON_DLL_BUILD)

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb adt)
