set(XENON_SRC 

	entryPoint.h
	core.h
	application.h
	application.cpp

	appData.h
	appData.cpp

	window.cpp
	window.h
	shader.cpp
	shader.h

)

include_directories(pch)

add_library(XenonEngine SHARED ${XENON_SRC})

target_compile_definitions(XenonEngine PUBLIC XENON_DLL_BUILD)

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb )