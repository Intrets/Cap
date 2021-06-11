function(make_module MODULE_NAME MODULE_FILES)
	foreach(FILE ${MODULE_FILES})
		list(APPEND SOURCES_LIST "${FILE}.cpp")
		list(APPEND HEADERS_LIST "include/${MODULE_NAME}/${FILE}.h")
	endforeach()

	add_library(${MODULE_NAME} ${SOURCES_LIST} ${HEADERS_LIST})

	target_include_directories(${MODULE_NAME} PUBLIC include)
	target_include_directories(${MODULE_NAME} PRIVATE include/${MODULE_NAME})

#	target_compile_options(${MODULE_NAME} PUBLIC -std:c++latest -Wno-sign-compare -Wno-deprecated-volatile -Wno-deprecated-declarations -Wno-deprecated-anon-enum-enum-conversion -Wno-ignored-qualifiers -ftime-trace)
	target_compile_options(${MODULE_NAME} PUBLIC -std:c++latest)

	source_group(
		TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}"
		PREFIX "Header Files"
		FILES ${HEADERS_LIST}
	)
endfunction()

