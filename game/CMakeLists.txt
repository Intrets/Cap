include("${PROJECT_SOURCE_DIR}/cmake/MakeModule.cmake")

set(MODULE_NAME game)

set(MODULE_FILES
	GameState
	player/PlayerInfo
	ui/ConstructDebugUI
	Test
)

set(GENERATE_STRUCTS
	Test
)

foreach(GEN ${GENERATE_STRUCTS})
	set(FULL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}/${GEN}.h")

	execute_process(
		COMMAND "C:/tools/msys64/usr/bin/python.exe" "${CMAKE_SOURCE_DIR}/scripts/meta.py" "-f${FULL_PATH}"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		RESULT_VARIABLE OUT
	)
endforeach()

make_module(${MODULE_NAME} "${MODULE_FILES}")

target_link_libraries(${MODULE_NAME} PRIVATE
	mem
	render
	ui
)
