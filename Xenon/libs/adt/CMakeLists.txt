cmake_minimum_required(VERSION 3.21...3.27 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
    ArgonDevTools
    VERSION 1.0.0
    DESCRIPTION "Tools used for development in Argon Interactive"
    LANGUAGES CXX C
)

set(SRC 
    logger.cpp
    logger.hpp
    async.cpp
    async.hpp
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(adt ${SRC})

# Library build
target_include_directories(adt PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(adt PUBLIC cxx_std_20)
target_compile_definitions(adt PUBLIC SAMPLE_LIBRARY_STATIC_DEFINE)
