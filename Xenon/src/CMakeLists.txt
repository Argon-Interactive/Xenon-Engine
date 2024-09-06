file(GLOB_RECURSE XENON_SRC "*.cpp" "*.hpp" "*.h")

if(xenon_BUILD_TESTS)
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Test")
	add_executable(XenonEngine ${XENON_SRC} ${XENON_TEST_SRC})
	target_include_directories(XenonEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${XENON_TEST_DIR})
	target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb adt Catch2::Catch2WithMain)
else()
	add_library(XenonEngine SHARED ${XENON_SRC})
	target_include_directories(XenonEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	target_compile_definitions(XenonEngine PUBLIC XENON_DLL_BUILD)
	target_link_libraries(XenonEngine PRIVATE xenon_options xenon_warnings glm glfw glad stb adt)
endif()

