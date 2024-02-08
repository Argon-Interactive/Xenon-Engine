set(XENON_SRC 

	xenon.cpp

)

add_library(XenonEngine SHARED ${XENON_SRC})

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings)