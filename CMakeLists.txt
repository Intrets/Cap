CMAKE_MINIMUM_REQUIRED(VERSION 3.20.3)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLM REQUIRED)
FIND_PACKAGE(GLFW3 REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(GLI REQUIRED)
set(SDL2_DIR "c:/program files (x86)/sdl2/cmake")
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL_Mixer REQUIRED)

PROJECT(game_thing LANGUAGES CXX)

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

add_subdirectory(app)

