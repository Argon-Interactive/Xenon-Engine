cmake_minimum_required(VERSION 3.1...3.27 FATAL_ERROR)
project(stb)

add_library(stb)
target_sources(stb PRIVATE stb.cpp)
target_include_directories(stb PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(stb PRIVATE -fPIC)
