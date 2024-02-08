function(xenon_prevent_out_of_source_builds)
	get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
	get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

	if("${srcdir}" STREQUAL "${bindir}")
		message("##################################################################")
		message("Error: builds inside source folder are forbidden")
		message("Create a separate build directory and run cmake from there.")
		message("##################################################################")
		message(FATAL_ERROR "Forbidden build target directory")
	endif()
endfunction()

xenon_prevent_out_of_source_builds()