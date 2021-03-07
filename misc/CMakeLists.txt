set(MODULE_NAME misc)

set(MODULE_FILES
	Rectangle
	NumHelpers
)

foreach(FILE ${MODULE_FILES})
	list(APPEND SOURCES_LIST "src/${FILE}.cpp")
	list(APPEND HEADERS_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}/${FILE}.h")
endforeach()

add_library(${MODULE_NAME} ${SOURCES_LIST} ${HEADERS_LIST})
add_library("${MODULE_NAME}::${MODULE_NAME}" ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME} PRIVATE "./include/${MODULE_NAME}")
target_include_directories(${MODULE_NAME} PUBLIC ./include)

target_link_libraries(${MODULE_NAME} PRIVATE GLM)

target_compile_features(${MODULE_NAME} PUBLIC cxx_std_11)

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}"
	PREFIX "Header Files"
	FILES ${HEADERS_LIST}
)
