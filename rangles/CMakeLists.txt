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

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	message(STATUS "Compiler: MSVC")
	add_compile_options(/FC /W4 /WX /std:c++latest)

	# disabled warnings
	add_compile_options(/wd4100) # 'identifier' : unreferenced formal parameter

	# enabled warnings
	add_compile_options(/w44062) # enumerator 'identifier' in a switch of enum 'enumeration' is not handled
	add_compile_definitions(COMPILER_MSVC)

elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)

	# clang-cl
	if(${CMAKE_CXX_SIMULATE_ID} STREQUAL MSVC)
		message(STATUS "Compiler: Clang-cl")
		add_compile_options(-W4 -Werror -std:c++20)

		# disabled warnings
		add_compile_options(-Wno-sign-compare)
		add_compile_options(-Wno-unused-parameter)

		add_compile_definitions(COMPILER_CLANGCL)

	# clang
	else()
		message(STATUS "Compiler: Clang")
		add_compile_options(-W4 -Werror -std=c++20)

		# disabled warnings
		add_compile_options(-Wno-sign-compare)
		add_compile_options(-Wno-unused-parameter)

		add_compile_definitions(COMPILER_CLANG)
		message(WARNING "Clang not tested")
	endif()

else()
	message(FATAL_ERROR "Untested/unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()
