add_executable(${PROJECT_NAME}
	main.cpp
	Setup.h
	Setup.cpp
	MainLoop.h
	MainLoop.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	opengl32
	GLFW3
	GLEW
	ui
	mem
	game
	misc
	render
	WGLM
	SDL2::SDL2-static
	SDL_Mixer
	sound
)
