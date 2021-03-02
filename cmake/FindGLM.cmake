SET(GLM_SEARCH_DIRS $ENV{PATH})

FIND_PATH(GLM_INCLUDE_DIR "glm/glm.hpp"
	PATHS GLM_SEARCH_DIRS)

if(GLM_INCLUDE_DIR)
	MESSAGE(STATUS "GLM include dir found: ${GLM_INCLUDE_DIR}")
else()
	MESSAGE(VERBOSE "GLM include dir searched for in: ${GLM_SEARCH_DIRS}")
	MESSAGE(FATAL_ERROR "GLM include dir not found. See above for paths searched in.")
endif()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	GLM
	DEFAULT_MSG
	GLM_INCLUDE_DIR
)

if(GLM_FOUND)
	MESSAGE(STATUS "GLM found.")
else()
	MESSAGE(FATAL_ERROR "GLM not found.")
endif()

add_library(GLM INTERFACE)
target_include_directories(GLM INTERFACE ${GLM_INCLUDE_DIR})
