include("${PROJECT_SOURCE_DIR}/cmake/MakeModule.cmake")

set(MODULE_NAME render)

set(MODULE_FILES
	Enums
	infos/RenderInfo
	infos/CameraInfo
	infos/TextRenderInfo
	infos/UIRenderInfo
	Colors
)

make_module(${MODULE_NAME} "${MODULE_FILES}")

target_link_libraries(${MODULE_NAME} PRIVATE
	GLM
	ui::ui
	misc::misc
	render::render
)
