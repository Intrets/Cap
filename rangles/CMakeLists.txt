cmake_minimum_required (VERSION 3.16)

set(MODULE_NAME rangles)

project(${MODULE_NAME} LANGUAGES CXX)

add_library(${MODULE_NAME} rangles.cpp include/rangles/rangles.h)

target_include_directories(${MODULE_NAME} PUBLIC include)
target_include_directories(${MODULE_NAME} PRIVATE include/${MODULE_NAME})

target_compile_features(${MODULE_NAME} PUBLIC cxx_std_20)

add_executable(rangles_test
	main.cpp
)

target_link_libraries(rangles PUBLIC
	tepp
)

target_link_libraries(rangles_test PUBLIC
	tepp
)
