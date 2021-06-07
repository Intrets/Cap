include("${PROJECT_SOURCE_DIR}/cmake/FindStaticLib.cmake")

find_static_lib(
	GLEW
	glew32
	GL/glew.h
)
