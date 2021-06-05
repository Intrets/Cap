add_executable(${PROJECT_NAME}
	main.cpp
	Setup.h
	Setup.cpp
	MainLoop.h
	MainLoop.cpp
)

# compile options also in MakeModule.cmake

# Full path of source code file in diagnostics
if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /FC)
endif()

if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /wd4100)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
	opengl32
	GLFW3
	GLEW
	ui
	mem
	game
	misc
	render
	GLM
)
