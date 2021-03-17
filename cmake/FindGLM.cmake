set(GLM_SEARCH_DIRS $ENV{PATH})

find_path(GLM_INCLUDE_DIR "glm/glm.hpp"
	PATHS GLM_SEARCH_DIRS)

if(GLM_INCLUDE_DIR)
	message(STATUS "GLM include dir found: ${GLM_INCLUDE_DIR}")
else()
	message(VERBOSE "GLM include dir searched for in: ${GLM_SEARCH_DIRS}")
	message(FATAL_ERROR "GLM include dir not found. See above for paths searched in.")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	GLM
	DEFAULT_MSG
	GLM_INCLUDE_DIR
)

if(GLM_FOUND)
	message(STATUS "GLM found.")
else()
	message(FATAL_ERROR "GLM not found.")
endif()

add_library(GLM_DO_NOT_USE INTERFACE)
target_include_directories(GLM_DO_NOT_USE INTERFACE ${GLM_INCLUDE_DIR})
