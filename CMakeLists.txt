cmake_minimum_required(VERSION 3.20.3)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Source dir same as build dir. Please don't use in-source builds. Remove or change CMakeCache.txt to change build dir.")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(game_thing LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# msvc
if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	message(STATUS "Compiler: MSVC")
	add_compile_options(/FC /W4 /WX /wd4100 /std:c++latest)
	add_compile_options(/w44062) # enumerator 'identifier' in a switch of enum 'enumeration' is not handled
	add_compile_definitions(COMPILER_MSVC)

elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)

	# clang-cl
	if(${CMAKE_CXX_SIMULATE_ID} STREQUAL MSVC)
		message(STATUS "compiler: Clang-cl")
		add_compile_options(-W4 -Werror -std:c++20 -Wno-sign-compare -Wno-unused-parameter)
		add_compile_definitions(COMPILER_CLANGCL)

	# clang
	else()
		message(STATUS "compiler: Clang")
		add_compile_options(-W4 -Werror -std=c++20 -Wno-sign-compare -Wno-unused-parameter)
		add_compile_definitions(COMPILER_CLANG)
		message(WARNING "Clang not tested")
	endif()

else()
	message(FATAL_ERROR "Untested/unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

include("${PROJECT_SOURCE_DIR}/cmake/FindStaticLib.cmake")


find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLI REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL_Mixer REQUIRED)


add_subdirectory(ui)
add_subdirectory(mem)
add_subdirectory(misc)
add_subdirectory(game)
add_subdirectory(render)
add_subdirectory(wrapped_glm)
add_subdirectory(tepp)
add_subdirectory(serial)
add_subdirectory(sound)

add_subdirectory(app)

