function(make_module MODULE_NAME MODULE_FILES)
	foreach(FILE ${MODULE_FILES})
		list(APPEND SOURCES_LIST "${FILE}.cpp")
		list(APPEND HEADERS_LIST "include/${MODULE_NAME}/${FILE}.h")
	endforeach()

	add_library(${MODULE_NAME} ${SOURCES_LIST} ${HEADERS_LIST})

	target_include_directories(${MODULE_NAME} PUBLIC include)
	target_include_directories(${MODULE_NAME} PRIVATE include/${MODULE_NAME})

	# target_compile_features(${MODULE_NAME} PUBLIC cxx_std_17)

	source_group(
		TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}"
		PREFIX "Header Files"
		FILES ${HEADERS_LIST}
	)

	# compile options also in app/CMakeLists.txt

	# Full path of source code file in diagnostics
	if (MSVC)
		target_compile_options(${MODULE_NAME} PRIVATE /FC)
	endif()

	if (MSVC)
		target_compile_options(${MODULE_NAME} PRIVATE /W4 /WX /wd4100)
	else()
		target_compile_options(${MODULE_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
	endif()

endfunction()

