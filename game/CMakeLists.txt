include("${PROJECT_SOURCE_DIR}/cmake/MakeModule.cmake")

set(MODULE_NAME game)

set(MODULE_FILES
	GameState
	player/PlayerInfo
	player/Inventory
	Signature
	GameObject
	Game
	Grapher
	WorldGrid
	Merger
	ui/ConstructDebugUI
	ui/game_control/EntityInterface
)

make_module(${MODULE_NAME} "${MODULE_FILES}")

target_link_libraries(${MODULE_NAME} PRIVATE
	mem
	render
	ui
)

target_link_libraries(${MODULE_NAME} PUBLIC
	tepp
	serial
)
