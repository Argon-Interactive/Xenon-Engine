cmake_minimum_required(VERSION 3.21...3.27 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
    XenonEngine
    VERSION 0.0.1
    DESCRIPTION "The Xenon Game Engine"
    # HOMEPAGE_URL "naszastronainternetowa.pl"
    LANGUAGES CXX C
)

include(cmake/PreventInSourceBuild.cmake)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ProjectOptions.cmake)
include(ProjectOptions.cmake)
xenon_options_setup()
xenon_apply_options()

target_compile_features(xenon_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Test")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Test")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Test")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(__DEBUG__)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(__DIST__)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	add_compile_definitions(__RELEASE__)
endif()

include_directories(Xenon/include)
include_directories(Xenon/src)
include_directories(Xenon/vendor/glad/include/glad)
include_directories(Xenon/vendor/glad/include/KHR)
include_directories(Xenon/vendor/glad/include)
include_directories(Xenon/vendor/glfw/include/GLFW)
include_directories(Xenon/vendor/glm)
include_directories(Xenon/vendor/stb)
include_directories(Xenon/vendor/all)

add_subdirectory(Xenon)
add_subdirectory(Sandbox)