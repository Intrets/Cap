SET(GLI_SEARCH_DIRS $ENV{PATH})

FIND_PATH(GLI_INCLUDE_DIR "gli.hpp"
	PATHS GLI_SEARCH_DIRS)

if(GLI_INCLUDE_DIR)
	MESSAGE(STATUS "GLI include dir found: ${GLI_INCLUDE_DIR}")
else()
	MESSAGE(VERBOSE "GLI include dir searched for in: ${GLI_SEARCH_DIRS}")
	MESSAGE(FATAL_ERROR "GLI include dir not found. See above for paths searched in.")
endif()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	GLI
	DEFAULT_MSG
	GLI_INCLUDE_DIR
)

if(GLI_FOUND)
	MESSAGE(STATUS "GLI found.")
else()
	MESSAGE(FATAL_ERROR "GLI not found.")
endif()

add_library(GLI INTERFACE)
target_include_directories(GLI INTERFACE ${GLI_INCLUDE_DIR})
