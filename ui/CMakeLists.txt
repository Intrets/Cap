set(MODULE_FILES
	UI
)

foreach(FILE ${MODULE_FILES})
	list(APPEND SOURCES_LIST "src/${FILE}.cpp")
	list(APPEND HEADERS_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include/ui/${FILE}.h")
endforeach()

add_library(ui ${SOURCES_LIST} ${HEADERS_LIST})

target_include_directories(ui PUBLIC ./include)

# target_link_libraries(ui PRIVATE ...)

target_compile_features(ui PUBLIC cxx_std_11)

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/ui"
	PREFIX "Header Files"
	FILES ${HEADERS_LIST}
)
