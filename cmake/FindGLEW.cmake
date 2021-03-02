SET(GLEW_SEARCH_DIRS $ENV{PATH})

FIND_PATH(
	GLEW_INCLUDE_DIR "GL/glew.h"
	PATHS GLEW_SEARCH_DIRS
)

if(GLEW_INCLUDE_DIR)
	MESSAGE(STATUS "GLEW include dir found: ${GLEW_INCLUDE_DIR}")
else()
	MESSAGE(VERBOSE "GLEW include dir searched for in: ${GLEW_SEARCH_DIRS}")
	MESSAGE(FATAL_ERROR "GLEW include dir not found. See above for paths searched in.")
endif()

if (WIN32)
	FIND_FILE(
		GLEW_LIB "glew32.lib"
		PATHS GLEW_SEARCH_DIRS
	)

	if (GLEW_LIB)
		message(status "glew32.lib found: ${glew32_lib}")
	else()
		message(verbose "glew32.lib searched for in: ${glew32_search_dirs}")
		message(fatal_error "glew32.lib not found. see above for paths searched in.")
	endif()

	FIND_FILE(
		GLEW_DLL "glew32.dll"
		PATHS GLEW_SEARCH_DIRS
	)

	if (GLEW_DLL)
		message(status "glew32.dll found: ${glew32_lib}")
	else()
		message(verbose "glew32.dll searched for in: ${glew32_search_dirs}")
		message(fatal_error "glew32.dll not found. see above for paths searched in.")
	endif()
else(WIN32)
	MESSAGE(FATAL_ERROR "Finding precompiled libs not implemented for non-windows.")
endif(WIN32)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	GLEW
	DEFAULT_MSG
	GLEW_INCLUDE_DIR
	GLEW_LIB
	GLEW_DLL
)

if(GLEW_FOUND)
	MESSAGE(STATUS "GLEW found.")
else()
	MESSAGE(FATAL_ERROR, "GLEW not found.")
endif()

add_library(GLEW SHARED IMPORTED)
set_target_properties(GLEW PROPERTIES
	IMPORTED_LOCATION ${GLEW_DLL}
	IMPORTED_IMPLIB ${GLEW_LIB}
	INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}"
)
