set(XENON_SRC 

	xenon.cpp
	window.cpp
	window.h
	shader.cpp
	shader.h
	core.h
 "application.h" "entryPoint.h" "application.cpp")

include_directories(pch)

add_library(XenonEngine SHARED ${XENON_SRC})

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb )