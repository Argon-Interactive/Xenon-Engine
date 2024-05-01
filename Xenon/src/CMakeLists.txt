set(XENON_SRC 
	api.h

	core/entryPoint.hpp
	core/application.cpp
	core/application.hpp
	core/appData.cpp
	core/appData.hpp
	core/globalData.cpp
	core/globalData.hpp
	core/event/event.cpp
	core/event/event.hpp

	window/window.cpp
	window/window.hpp

	rendering/shader.cpp
	rendering/shader.hpp
	rendering/renderer2D_core.cpp
	rendering/renderer2D_core.hpp
	rendering/texture_core.cpp
	rendering/texture_core.hpp

	devTools/timer.hpp
	devTools/logger.cpp
	devTools/logger.hpp
	devTools/xenonDevTools.cpp

	temporary/engDataStruct.hpp

	test/test.cpp
	test/test.hpp

 )

add_library(XenonEngine SHARED ${XENON_SRC})

target_include_directories(XenonEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(XenonEngine PUBLIC XENON_DLL_BUILD)

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb)
