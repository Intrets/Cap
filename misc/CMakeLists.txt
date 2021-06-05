include("${PROJECT_SOURCE_DIR}/cmake/MakeModule.cmake")

set(MODULE_NAME misc)

set(MODULE_FILES
	Rectangle
	NumHelpers
	Misc
	UpdatedCache
	StringHelpers
	Option
	FunctionHelpers
	IOWrapped
	PathManager
	Enums
	Log
	Timer
	PeriodAverage
	CircularArray
)

make_module(${MODULE_NAME} "${MODULE_FILES}")

target_link_libraries(${MODULE_NAME} PUBLIC
	GLM
)

target_link_libraries(${MODULE_NAME} PRIVATE
	GLEW
	GLFW3
	mem
	render
)
