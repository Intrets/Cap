# CMakeList.txt : CMake project for Game, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project (Game)

file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/src/*.hpp)

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add source to this project's executable.
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} opengl32)

find_package(GLM REQUIRED)
target_link_libraries(${PROJECT_NAME} GLM)

find_package(GLFW3 REQUIRED)
target_link_libraries(${PROJECT_NAME} GLFW3)

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} GLEW)

find_package(GLI REQUIRED)
target_link_libraries(${PROJECT_NAME} GLI)


