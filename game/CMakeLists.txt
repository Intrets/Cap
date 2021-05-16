include("${PROJECT_SOURCE_DIR}/cmake/MakeModule.cmake")

set(MODULE_NAME game)

set(MODULE_FILES
	GameState
	player/PlayerInfo
	ui/ConstructDebugUI
	GameObject
	Signature
)

set(GENERATE_STRUCTS
	GameObject
)

foreach(GEN ${GENERATE_STRUCTS})
	set(FULL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/${MODULE_NAME}/${GEN}.h")
	set(FULL_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${GEN}.cpp")

	execute_process(
		COMMAND "C:/tools/msys64/usr/bin/python.exe" "${CMAKE_SOURCE_DIR}/scripts/meta.py" "-f${FULL_PATH}" "-s${FULL_SOURCE_PATH}"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	)
endforeach()

make_module(${MODULE_NAME} "${MODULE_FILES}")

target_link_libraries(${MODULE_NAME} PRIVATE
	mem
	render
	ui
)
