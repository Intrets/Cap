function(find_interface_lib LIB_NAME INCLUDE_FILE)
	set(LIB ${LIB_NAME}_LIB)
	set(INCLUDE_DIR ${LIB_NAME}_INCLUDE_DIR)

	find_path(
		${INCLUDE_DIR} ${INCLUDE_FILE}
	)

	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(
		${LIB_NAME} DEFAULT_MSG
		${INCLUDE_DIR}
	)

	add_library(${LIB_NAME} INTERFACE)

	target_include_directories(${LIB_NAME} INTERFACE ${${INCLUDE_DIR}})

	if(${${LIB_NAME}_FOUND})
		message("-- ${LIB_NAME} found.")
	endif()
endfunction()
