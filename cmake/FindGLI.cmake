set(GLI_SEARCH_DIRS $ENV{PATH})

find_path(GLI_INCLUDE_DIR "gli.hpp"
	PATHS GLI_SEARCH_DIRS)

if(GLI_INCLUDE_DIR)
	message(STATUS "GLI include dir found: ${GLI_INCLUDE_DIR}")
else()
	message(VERBOSE "GLI include dir searched for in: ${GLI_SEARCH_DIRS}")
	message(FATAL_ERROR "GLI include dir not found. See above for paths searched in.")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	GLI
	DEFAULT_MSG
	GLI_INCLUDE_DIR
)

if(GLI_FOUND)
	message(STATUS "GLI found.")
else()
	message(FATAL_ERROR "GLI not found.")
endif()

add_library(GLI INTERFACE)
target_include_directories(GLI INTERFACE ${GLI_INCLUDE_DIR})
