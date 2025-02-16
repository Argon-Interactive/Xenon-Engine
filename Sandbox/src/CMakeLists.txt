add_executable(sandbox main.cpp)

target_link_libraries(sandbox PRIVATE XenonEngine xenon_options xenon_warnings adt)
add_dependencies(sandbox copy_xenon_dll copy_assets)
