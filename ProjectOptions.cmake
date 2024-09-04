include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

macro(xenon_sanitizers_support)
    if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
        set(SUPPORTS_UBSAN ON)
    else()
        set(SUPPORTS_UBSAN OFF)
    endif()
    if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
        set(SUPPORTS_ASAN OFF)
    else()
        set(SUPPORTS_ASAN ON)
    endif()
endmacro()

macro(xenon_options_setup)
	option(xenon_BUILD_TESTS "Build tests" ON)
	option(xenon_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
	option(xenon_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
	option(xenon_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
	option(xenon_ENABLE_PCH "Enable precompiled headers" OFF)
    #TODO:
    option(xenon_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(xenon_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(xenon_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(xenon_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(xenon_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(xenon_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)

    mark_as_advanced(
        xenon_WARNINGS_AS_ERRORS
        xenon_ENABLE_CLANG_TIDY
        xenon_ENABLE_CPPCHECK
        xenon_ENABLE_IPO
        xenon_ENABLE_SANITIZER_ADDRESS
        xenon_ENABLE_SANITIZER_LEAK
        xenon_ENABLE_SANITIZER_UNDEFINED
        xenon_ENABLE_SANITIZER_THREAD
        xenon_ENABLE_SANITIZER_MEMORY
		xenon_ENABLE_PCH
    )
endmacro()

macro(xenon_apply_options)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

	add_library(xenon_warnings INTERFACE)
	add_library(xenon_options INTERFACE)

	if(NOT ${xenon_WARNINGS_AS_ERRORS})
		message("###################################################################")
		message("#   Warning: compiler warnings not treated as errors              #")
		message("#   It is recommended to set this option to ON                    #")
		message("###################################################################")
	endif()

	include(cmake/CompilerWarnings.cmake)
	set_project_warnings(xenon_warnings ${xenon_WARNINGS_AS_ERRORS} "" "" "" "")

	include(cmake/ClangTidy.cmake)
    if(xenon_ENABLE_CLANG_TIDY)
        xenon_enable_clang_tidy(xenon_options ${xenon_WARNINGS_AS_ERRORS})
    endif()

    include(cmake/CppCheck.cmake)
    if(xenon_ENABLE_CPPCHECK)
        xenon_enable_cppcheck(${xenon_WARNINGS_AS_ERRORS} "")
    endif()

	if(xenon_ENABLE_PCH)
		target_precompile_headers(
			xenon_options
			INTERFACE
			<utility>
			<fstream>
			<algorithm>
			<filesystem>
			<thread>
			<chrono>
			<functional>
			<memory>
			<cmath>
			<numeric>
			<vector>
			<array>
			<string>
			<map>
			<unordered_map>
			<initializer_list>
			<queue>
			<list>
			<cstdint>
			<stdexcept>)
	endif()

	if(${xenon_BUILD_TESTS})
		message("###################################################################")
		message("#                         Building tests                          #")
		message("###################################################################")
		find_package(Catch2 3 REQUIRED)
	endif()

endmacro()
