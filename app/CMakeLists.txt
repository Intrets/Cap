add_executable(${PROJECT_NAME}
	main.cpp
	Setup.h
	Setup.cpp
	MainLoop.h
	MainLoop.cpp
)

option(USE_PRECOMPILED_HEADERS "enable/disable precompiled headers" ON)

if(USE_PRECOMPILED_HEADERS)
	target_precompile_headers(game PUBLIC pch.h)
	target_precompile_headers(render PUBLIC pch.h)
	target_precompile_headers(serial PUBLIC pch.h)
	target_precompile_headers(ui PUBLIC pch.h)
	target_precompile_headers(misc PUBLIC pch.h)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
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
