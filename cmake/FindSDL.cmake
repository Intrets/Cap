set(SDL_SEARCH_DIRS $ENV{PATH})

find_path(SDL_INCLUDE_DIR "include/SDL.h"
	PATHS SDL_SEARCH_DIRS)

if(SDL_INCLUDE_DIR)
	message(STATUS "SDL include dir found: ${SDL_INCLUDE_DIR}")
else()
	message(STATUS "SDL include dir searched for in: ${SDL_SEARCH_DIRS}")
	message(FATAL_ERROR "SDL include dir not found. See above for paths searched in.")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	SDL
	DEFAULT_MSG
	SDL_INCLUDE_DIR
)

if(SDL_FOUND)
	message(STATUS "SDL found.")
else()
	message(FATAL_ERROR "SDL not found.")
endif()

add_library(SDL INTERFACE)
target_include_directories(SDL INTERFACE ${SDL_INCLUDE_DIR})
