
add_executable(${PROJECT_NAME}
	main.cpp
	Setup.h
	Setup.cpp
	MainLoop.h
	MainLoop.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE
	ui
	mem
	game
	GLFW3
	GLEW
	opengl32
)
