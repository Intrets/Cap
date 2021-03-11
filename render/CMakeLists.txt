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

set(SHADER_FILES
	Blit
)

foreach(SHADER ${SHADER_FILES})
	set(FULL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER}")

	file(READ "${FULL_PATH}.frag" FRAGMENT_SHADER)
	file(WRITE "${FULL_PATH}.frag.inc" "const char* ${SHADER}_frag = R\"(${FRAGMENT_SHADER})\";")

	file(READ "${FULL_PATH}.vert" VERTEX_SHADER)
	file(WRITE "${FULL_PATH}.vert.inc" "const char* ${SHADER}_vert = R\"(${VERTEX_SHADER})\";")
endforeach()


make_module(${MODULE_NAME} "${MODULE_FILES}")

target_include_directories(${MODULE_NAME} PRIVATE "./shaders")

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
