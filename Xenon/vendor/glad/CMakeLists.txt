cmake_minimum_required(VERSION 3.1...3.27 FATAL_ERROR)
project(glad)

add_library(glad)
target_sources(glad PRIVATE src/glad.c)
target_include_directories(glad PUBLIC include)
target_compile_options(glad PRIVATE -fPIC)
