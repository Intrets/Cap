set(SDL_Mixer_SEARCH_DIRS $ENV{PATH})

find_path(
	SDL_Mixer_INCLUDE_DIR "SDL_Mixer.h"
	PATHS SDL_Mixer_SEARCH_DIRS
)

if(SDL_Mixer_INCLUDE_DIR)
	message(STATUS "SDL_Mixer include dir found: ${SDL_Mixer_INCLUDE_DIR}")
else()
	message(VERBOSE "SDL_Mixer include dir searched for in: ${SDL_Mixer_SEARCH_DIRS}")
	message(FATAL_ERROR "SDL_Mixer include dir not found. See above for paths searched in.")
endif()

if (WIN32)
	find_file(
		SDL_Mixer_LIB "SDL2_Mixer.lib"
		PATHS SDL_Mixer_SEARCH_DIRS
	)

	if (SDL_Mixer_LIB)
		message(STATUS "SDL2_Mixer.lib found: ${SDL_Mixer_LIB}")
	else()
		message(VERBOSE "SDL2_Mixer.lib searched for in: ${SDL2_Mixer_search_dirs}")
		message(FATAL_ERROR "SDL2_Mixer.lib not found. see above for paths searched in.")
	endif()

	find_file(
		SDL_Mixer_DLL "SDL2_Mixer.dll"
		PATHS SDL_Mixer_SEARCH_DIRS
	)

	if (SDL_Mixer_DLL)
		message(STATUS "SDL2_Mixer.dll found: ${SDL_Mixer_DLL}")
	else()
		message(VERBOSE "SDL2_Mixer.dll searched for in: ${SDL2_Mixer_search_dirs}")
		message(FATAL_ERROR "SDL2_Mixer.dll not found. see above for paths searched in.")
	endif()
else(WIN32)
	message(FATAL_ERROR "Finding precompiled libs not implemented for non-windows.")
endif(WIN32)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	SDL_Mixer
	DEFAULT_MSG
	SDL_Mixer_INCLUDE_DIR
	SDL_Mixer_LIB
	SDL_Mixer_DLL
)

if(SDL_Mixer_FOUND)
	message(STATUS "SDL_Mixer found.")
else()
	message(FATAL_ERROR "SDL_Mixer not found.")
endif()

add_library(SDL_Mixer STATIC IMPORTED)
set_target_properties(SDL_Mixer PROPERTIES
	IMPORTED_LOCATION ${SDL_Mixer_LIB}
	IMPORTED_IMPLIB ${SDL_Mixer_LIB}
	INTERFACE_INCLUDE_DIRECTORIES "${SDL_Mixer_INCLUDE_DIR}"
)
