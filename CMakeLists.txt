cmake_minimum_required(VERSION 3.20.3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(game_thing LANGUAGES CXX)

if (MSVC)
	add_compile_options(/FC /W4 /WX /wd4100)
else()
	add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

include("${PROJECT_SOURCE_DIR}/cmake/FindStaticLib.cmake")

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLI REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL_Mixer REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

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

