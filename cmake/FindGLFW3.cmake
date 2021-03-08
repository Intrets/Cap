set(GLFW3_SEARCH_DIRS $ENV{PATH})

find_path(
	GLFW3_INCLUDE_DIR "GLFW/glfw3.h"
	PATHS GLFW3_SEARCH_DIRS
)

if(GLFW3_INCLUDE_DIR)
	message(STATUS "GLFW3 include dir found: ${GLFW3_INCLUDE_DIR}")
else()
	message(VERBOSE "GLFW3 include dir searched for in: ${GLFW3_SEARCH_DIRS}")
	message(FATAL_ERROR "GLFW3 include dir not found. See above for paths searched in.")
endif()

if (WIN32)
	find_file(
		GLFW3_LIB "glfw3.lib"
		PATHS GLFW3_SEARCH_DIRS
	)

	if (GLFW3_LIB)
		message(STATUS "glfw3.lib found: ${GLFW3_LIB}")
	else()
		message(VERBOSE "glfw3.lib searched for in: ${glfw3_search_dirs}")
		message(FATAL_ERROR "glfw3.lib not found. see above for paths searched in.")
	endif()

	find_file(
		GLFW3_DLL "glfw3.dll"
		PATHS GLFW3_SEARCH_DIRS
	)

	if (GLFW3_DLL)
		message(STATUS "glfw3.dll found: ${GLFW3_DLL}")
	else()
		message(VERBOSE "glfw3.dll searched for in: ${glfw3_search_dirs}")
		message(FATAL_ERROR "glfw3.dll not found. see above for paths searched in.")
	endif()
else(WIN32)
	message(FATAL_ERROR "Finding precompiled libs not implemented for non-windows.")
endif(WIN32)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	GLFW3
	DEFAULT_MSG
	GLFW3_INCLUDE_DIR
	GLFW3_LIB
	GLFW3_DLL
)

if(GLFW3_FOUND)
	message(STATUS "GLFW3 found.")
else()
	message(FATAL_ERROR "GLFW3 not found.")
endif()

add_library(GLFW3 STATIC IMPORTED)
set_target_properties(GLFW3 PROPERTIES
	IMPORTED_LOCATION ${GLFW3_LIB}
	IMPORTED_IMPLIB ${GLFW3_LIB}
	INTERFACE_INCLUDE_DIRECTORIES "${GLFW3_INCLUDE_DIR}"
)
