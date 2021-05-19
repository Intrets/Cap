include("${PROJECT_SOURCE_DIR}/cmake/MakeModule.cmake")

set(MODULE_NAME templates)

set(MODULE_FILES
	Everything
)

make_module(${MODULE_NAME} "${MODULE_FILES}")
