set(GLEW_SEARCH_DIRS $ENV{PATH})

find_path(
	GLEW_INCLUDE_DIR "GL/glew.h"
	PATHS GLEW_SEARCH_DIRS
)

if(GLEW_INCLUDE_DIR)
	Message(STATUS "GLEW include dir found: ${GLEW_INCLUDE_DIR}")
else()
	message(VERBOSE "GLEW include dir searched for in: ${GLEW_SEARCH_DIRS}")
	message(FATAL_ERROR "GLEW include dir not found. See above for paths searched in.")
endif()

if (WIN32)
	find_file(
		GLEW_LIB "glew32.lib"
		PATHS GLEW_SEARCH_DIRS
	)

	if (GLEW_LIB)
		message(STATUS "glew32.lib found: ${GLEW_LIB}")
	else()
		message(verbose "glew32.lib searched for in: ${glew32_search_dirs}")
		message(fatal_error "glew32.lib not found. see above for paths searched in.")
	endif()

	find_file(
		GLEW_DLL "glew32.dll"
		PATHS GLEW_SEARCH_DIRS
	)

	if (GLEW_DLL)
		message(STATUS "glew32.dll found: ${GLEW_DLL}")
	else()
		message(VERBOSE "glew32.dll searched for in: ${glew32_search_dirs}")
		message(FATAL_ERROR "glew32.dll not found. see above for paths searched in.")
	endif()
else(WIN32)
	message(FATAL_ERROR "Finding precompiled libs not implemented for non-windows.")
endif(WIN32)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	GLEW
	DEFAULT_MSG
	GLEW_INCLUDE_DIR
	GLEW_LIB
	GLEW_DLL
)

if(GLEW_FOUND)
	message(STATUS "GLEW found.")
else()
	message(FATAL_ERROR "GLEW not found.")
endif()

add_library(GLEW STATIC IMPORTED)
set_target_properties(GLEW PROPERTIES
	IMPORTED_LOCATION ${GLEW_LIB}
	IMPORTED_IMPLIB ${GLEW_LIB}
	INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}"
)
