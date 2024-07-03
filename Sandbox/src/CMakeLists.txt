add_executable(sandbox main.cpp)

target_link_libraries(sandbox PRIVATE XenonEngine xenon_options xenon_warnings lib_adt)