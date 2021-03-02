SET(GLFW3_SEARCH_DIRS $ENV{PATH})

FIND_PATH(
	GLFW3_INCLUDE_DIR "GLFW/glfw3.h"
	PATHS GLFW3_SEARCH_DIRS
)

if(GLFW3_INCLUDE_DIR)
	MESSAGE(STATUS "GLFW3 include dir found: ${GLFW3_INCLUDE_DIR}")
else()
	MESSAGE(VERBOSE "GLFW3 include dir searched for in: ${GLFW3_SEARCH_DIRS}")
	MESSAGE(FATAL_ERROR "GLFW3 include dir not found. See above for paths searched in.")
endif()

if (WIN32)
	FIND_FILE(
		GLFW3_LIB "glfw3.lib"
		PATHS GLFW3_SEARCH_DIRS
	)

	if (GLFW3_LIB)
		message(status "glfw3.lib found: ${glfw3_lib}")
	else()
		message(verbose "glfw3.lib searched for in: ${glfw3_search_dirs}")
		message(fatal_error "glfw3.lib not found. see above for paths searched in.")
	endif()

	FIND_FILE(
		GLFW3_DLL "glfw3.dll"
		PATHS GLFW3_SEARCH_DIRS
	)

	if (GLFW3_DLL)
		message(status "glfw3.dll found: ${glfw3_lib}")
	else()
		message(verbose "glfw3.dll searched for in: ${glfw3_search_dirs}")
		message(fatal_error "glfw3.dll not found. see above for paths searched in.")
	endif()
else(WIN32)
	MESSAGE(FATAL_ERROR "Finding precompiled libs not implemented for non-windows.")
endif(WIN32)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	GLFW3
	DEFAULT_MSG
	GLFW3_INCLUDE_DIR
	GLFW3_LIB
	GLFW3_DLL
)

if(GLFW3_FOUND)
	MESSAGE(STATUS "GLFW3 found.")
else()
	MESSAGE(FATAL_ERROR, "GLFW3 not found.")
endif()

add_library(GLFW3 SHARED IMPORTED)
set_target_properties(GLFW3 PROPERTIES
	IMPORTED_LOCATION ${GLFW3_DLL}
	IMPORTED_IMPLIB ${GLFW3_LIB}
	INTERFACE_INCLUDE_DIRECTORIES "${GLFW3_INCLUDE_DIR}"
)
