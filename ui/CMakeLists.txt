include("${PROJECT_SOURCE_DIR}/cmake/MakeModule.cmake")

set(MODULE_NAME ui)

set(MODULE_FILES
	ui
	Base
	ControlState
	Enums
	State
	Constructer
	Proxy
	TextDisplay
	impl/Text
	binds/Binds
	SizeType
	ConstrainSize
	Button
	Pad
	Window
	"List"
	Empty
	ColoredBackground
	AnchoredProxy
	Destructible
)

make_module(${MODULE_NAME} "${MODULE_FILES}")

target_link_libraries(${MODULE_NAME} PRIVATE
	GLM
	GLEW
	GLFW3
	mem::mem
	misc::misc
	game::game
	render::render
)
