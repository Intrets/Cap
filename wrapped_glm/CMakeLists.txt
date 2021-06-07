add_library(WGLM INTERFACE)

target_include_directories(WGLM INTERFACE include)

target_link_libraries(WGLM INTERFACE GLM)
