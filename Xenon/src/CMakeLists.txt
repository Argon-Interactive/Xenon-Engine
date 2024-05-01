set(XENON_SRC 
	api.h

	core/appData.cpp
	core/appData.h
	core/application.cpp
	core/application.h
	core/entryPoint.h
	core/globalData.hpp
	core/globalData.cpp
	core/event/event.cpp
	core/event/event.hpp

	window/window.cpp
	window/window.h
	rendering/shader.cpp
	rendering/shader.h
	rendering/renderer2D_core.hpp
	rendering/renderer2D_core.cpp
	rendering/texture_core.cpp
	rendering/texture_core.hpp
	devTools/timer.hpp
	devTools/logger.hpp
	devTools/logger.cpp
	devTools/xenonDevTools.cpp
	temporary/engDataStruct.hpp
	test/test.cpp
 )

include_directories(core)
include_directories(devTools)
include_directories(pch)
include_directories(rendering)
include_directories(window)
include_directories(temporary)

add_library(XenonEngine SHARED ${XENON_SRC})

target_compile_definitions(XenonEngine PUBLIC XENON_DLL_BUILD)

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb)
