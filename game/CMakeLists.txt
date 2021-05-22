include("${PROJECT_SOURCE_DIR}/cmake/MakeModule.cmake")

set(MODULE_NAME game)

set(MODULE_FILES
	GameState
	player/PlayerInfo
	ui/ConstructDebugUI
	Signature
	GameObject
	Game
	entt
)

make_module(${MODULE_NAME} "${MODULE_FILES}")

target_link_libraries(${MODULE_NAME} PRIVATE
	mem
	render
	ui
)

target_link_libraries(${MODULE_NAME} PUBLIC
	tepp
)
