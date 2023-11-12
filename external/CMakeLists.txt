# Copyright (c) Johnny Patterson

include(ExternalProject)

option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_DOCUMENT_INTERNALS "" OFF)
option(GLFW_INSTALL "" OFF)
add_subdirectory(glfw EXCLUDE_FROM_ALL)
set_target_properties(glfw PROPERTIES FOLDER "External")

add_library(imgui STATIC)
set(IMGUI_DIR imgui)
file(GLOB imgui-src ${IMGUI_DIR}/*.h ${IMGUI_DIR}/*.cpp)
target_sources(imgui PRIVATE
	${imgui-src}
	${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_DIR}/backends/imgui_impl_glfw.h
	${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
	${IMGUI_DIR}/backends/imgui_impl_vulkan.h
)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw Vulkan::Vulkan)
set_target_properties(imgui PROPERTIES FOLDER "External")
