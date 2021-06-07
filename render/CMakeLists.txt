include("${PROJECT_SOURCE_DIR}/cmake/MakeModule.cmake")

set(MODULE_NAME render)

set(MODULE_FILES
	Enums
	Colors
	BufferWrappers
	loaders/ShaderLoader
	loaders/ModelLoader
	loaders/TextureLoader
	loaders/vboindexer
	BlitRenderer
	Renderer
	UIBackgroundRenderer
	TextRenderer
	DebugRenderer
	BlitRendererArray
	infos/RenderInfo
	infos/TextRenderInfo
	infos/UIRenderInfo
	infos/CameraInfo
	infos/TileRenderInfo
	infos/BlitArrayRenderInfo
	infos/BlitRenderInfo
	infos/DebugRenderInfo
	infos/HighlightRenderInfo
	Fonts
	textures/BlockIDTextures
	GLStateWrapper
	HighlightRenderer
)

set(SHADER_FILES
	Blit
	UIBackground
	BlitArray
	Debug
	Highlight
)

foreach(SHADER ${SHADER_FILES})
	set(FULL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER}")

	file(READ "${FULL_PATH}.frag" FRAGMENT_SHADER)
	file(WRITE "${FULL_PATH}.frag.inc" "static const char* ${SHADER}_frag = R\"(${FRAGMENT_SHADER})\";\n")

	file(READ "${FULL_PATH}.vert" VERTEX_SHADER)
	file(WRITE "${FULL_PATH}.vert.inc" "static const char* ${SHADER}_vert = R\"(${VERTEX_SHADER})\";\n")
endforeach()

make_module(${MODULE_NAME} "${MODULE_FILES}")

target_include_directories(${MODULE_NAME} PRIVATE "./shaders")

target_link_libraries(${MODULE_NAME} PUBLIC
	WGLM
	GLEW
	misc
)

target_link_libraries(${MODULE_NAME} PRIVATE
	GLI
	GLFW3
	mem
)
