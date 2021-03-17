add_library(GLM INTERFACE)

target_include_directories(GLM INTERFACE include)

target_link_libraries(GLM INTERFACE GLM_DO_NOT_USE)
