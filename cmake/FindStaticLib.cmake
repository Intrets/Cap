function(find_static_lib LIB_NAME LIB_FILE INCLUDE_FILE)
	set(LIB ${LIB_NAME}_LIB)
	set(INCLUDE_DIR ${LIB_NAME}_INCLUDE_DIR)

	find_path(
		${INCLUDE_DIR} ${INCLUDE_FILE}
	)

	find_library(
		${LIB} ${LIB_FILE}
	)

	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(
		${LIB_NAME} DEFAULT_MSG
		${INCLUDE_DIR}
		${LIB}
	)

	add_library(${LIB_NAME} STATIC IMPORTED)
	set_target_properties(${LIB_NAME} PROPERTIES
		IMPORTED_LOCATION ${${LIB}}
		IMPORTED_IMPLIB ${${LIB}}
		INTERFACE_INCLUDE_DIRECTORIES ${${INCLUDE_DIR}}
	)

	if(${${LIB_NAME}_FOUND})
		message("-- ${LIB_NAME} found.")
	endif()
endfunction()