set(XENON_SRC 
	core.h
	window/window.cpp
	window/window.h
	rendering/shader.cpp
	rendering/shader.h
	rendering/renderer2d.h
	rendering/renderer2d.cpp
	devTools/timer.hpp
	devTools/logger.hpp
	devTools/logger.cpp
	devTools/xenonDevTools.cpp
	core/appData.cpp
	core/appData.h
	core/application.cpp
	core/application.h
	core/entryPoint.h
)

include_directories(core)
include_directories(devTools)
include_directories(pch)
include_directories(rendering)
include_directories(window)

add_library(XenonEngine SHARED ${XENON_SRC})

target_compile_definitions(XenonEngine PUBLIC XENON_DLL_BUILD)

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb )