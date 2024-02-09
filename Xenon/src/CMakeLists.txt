set(XENON_SRC 

	xenon.cpp
	window.cpp
	window.h
	"rendering/shader.cpp"
	"rendering/shader.h"
 "rendering/renderer2d.cpp" "rendering/renderer2d.h" "dataStructures.h")

add_library(XenonEngine SHARED ${XENON_SRC})

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb )