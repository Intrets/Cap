include("${PROJECT_SOURCE_DIR}/cmake/MakeModule.cmake")

set(MODULE_NAME render)

set(MODULE_FILES
	Enums
	infos/RenderInfo
	infos/CameraInfo
	infos/TextRenderInfo
	infos/UIRenderInfo
	Colors
	Fonts
	BufferWrappers
	loaders/ShaderLoader
	loaders/ModelLoader
	loaders/TextureLoader
	loaders/vboindexer
	BlitRenderer
	GLEnabler
)

make_module(${MODULE_NAME} "${MODULE_FILES}")

target_link_libraries(${MODULE_NAME} PRIVATE
	GLM
	GLI
	GLEW
	GLFW3
	ui::ui
	misc::misc
	render::render
	mem::mem
)
