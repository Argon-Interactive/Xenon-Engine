macro(xenon_enable_cppcheck WARNINGS_AS_ERRORS CPPCHECK_OPTIONS)
	find_program(CPPCHECK cppcheck)
	if(CPPCHECK)

		if(CMAKE_GENERATOR MATCHES ".*Visual Studio.*")
			set(CPPCHECK_TEMPLATE "vs")
		else()
			set(CPPCHECK_TEMPLATE "gcc")
		endif()

		if("${CPPCHECK_OPTIONS}" STREQUAL "")
			set(CMAKE_CXX_CPPCHECK
				${CPPCHECK}
				--template=${CPPCHECK_TEMPLATE}
				--enable=style,performance,warning,portability
				--inline-suppr
				--suppress=cppcheckError
				--suppress=internalAstError
				--suppress=unmatchedSuppression
				--suppress=passedByValue
				--suppress=syntaxError
				--suppress=preprocessorErrorDirective
				--inconclusive)
		else()
			set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --template=${CPPCHECK_TEMPLATE} ${CPPCHECK_OPTIONS})
		endif()

		if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
			set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} --std=c++${CMAKE_CXX_STANDARD})
		endif()
		if(${WARNINGS_AS_ERRORS})
			list(APPEND CMAKE_CXX_CPPCHECK --error-exitcode=2)
		endif()
	else()
		message(${WARNING_MESSAGE} "cppcheck requested but executable not found")
	endif()
endmacro()
