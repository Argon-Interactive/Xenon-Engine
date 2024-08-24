project(Sandbox)

if(WIN32)
    set (DLL_FILE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/XenonEngine.dll")
endif()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Application")

add_subdirectory(src)

if(WIN32)
    add_custom_target(copy_xenon_dll ALL
        COMMAND ${CMAKE_COMMAND} -E copy
            ${DLL_FILE_PATH}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	    COMMENT "Copy assets folder to executable"
    )

    add_dependencies(copy_xenon_dll XenonEngine)
endif()

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
	COMMENT "Copy assets folder to executable"
)