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

file(TIMESTAMP "./shaders/Blit.frag" FRAG "%Y-%m-%dT%H:%M:%SZ")
file(WRITE "./shaders/Blit.frag.inc" ${FRAG})
file(TIMESTAMP "./shaders/Blit.frag.inc" TIME)

message(STATUS "timescamp ${FRAG}")
message(STATUS ${CMAKE_VERSION})

#message(STATUS ${FRAG})

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
