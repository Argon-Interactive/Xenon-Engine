set(XENON_SRC 
	api.h

	core/entryPoint.hpp
	core/application.cpp
	core/application.hpp
	core/appData.cpp
	core/appData.hpp
	core/event/event.cpp
	core/event/event.hpp
	core/input/input.hpp
	core/input/input.cpp

	window/window.cpp
	window/window.hpp

	devTools/timer.hpp
	devTools/logger/logger.cpp
	devTools/logger/logger.hpp
	devTools/logger/logger_core.hpp
	devTools/logger/logger_client.hpp
	
 )

add_library(XenonEngine SHARED ${XENON_SRC})

target_include_directories(XenonEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(XenonEngine PUBLIC XENON_DLL_BUILD)

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb)
