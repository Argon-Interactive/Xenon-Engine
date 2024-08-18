file(GLOB_RECURSE XENON_SRC "*.cpp" "*.hpp" "*.h")

add_library(XenonEngine SHARED ${XENON_SRC})

target_include_directories(XenonEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(XenonEngine PUBLIC XENON_DLL_BUILD)

target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb adt)
