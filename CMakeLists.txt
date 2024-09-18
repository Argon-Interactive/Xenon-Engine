cmake_minimum_required(VERSION 3.21...3.27 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
    XenonEngine
    VERSION 0.2.0
    DESCRIPTION "The Xenon Game Engine"
    # HOMEPAGE_URL ""
    LANGUAGES CXX C
)

include(cmake/PreventInSourceBuild.cmake)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()
if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()
if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(__DEBUG__)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(__DIST__)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	add_compile_definitions(__RELEASE__)
endif()

if(${USE_COMPILER} STREQUAL "cl")
	add_compile_definitions(__COMPILER_CL__)
elseif(${USE_COMPILER} STREQUAL "clang-cl")
	add_compile_definitions(__COMPILER_CLANG_CL__)
elseif(${USE_COMPILER} STREQUAL "gcc")
	add_compile_definitions(__COMPILER_GCC__)
elseif(${USE_COMPILER} STREQUAL "clang")
	add_compile_definitions(__COMPILER_CLANG__)
endif()

add_compile_definitions(GLM_FORCE_CXX11)
add_compile_definitions(GLM_FORCE_SILENT_WARNINGS)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ProjectOptions.cmake)
include(ProjectOptions.cmake)
xenon_options_setup()
xenon_apply_options()
target_compile_features(xenon_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

include_directories(Xenon/include)
include_directories(Xenon/src/all)
include_directories(Xenon/libs/all)
include_directories(Xenon/vendor/all)

if(${xenon_BUILD_TESTS})
	add_subdirectory(Tests)
else()
	add_subdirectory(Sandbox)
endif()

add_subdirectory(Xenon)
