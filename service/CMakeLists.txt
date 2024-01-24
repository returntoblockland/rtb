# Copyright (c) Johnny Patterson
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(rtb-service)
target_compile_features(rtb-service PRIVATE cxx_std_20)

target_include_directories(rtb-service PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)

file(GLOB_RECURSE src *)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${src})
target_sources(rtb-service PRIVATE
	${src}
	../external/imgui_glfw_vulkan.cpp
)

target_link_libraries(rtb-service PRIVATE
	imgui
	nlohmann_json
	Taskflow
	Vulkan::Vulkan
)

if(MSVC)
	set_target_properties(rtb-service PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endif()

if(WIN32)
	target_link_libraries(rtb-service PRIVATE
		Shell32
	)
endif()
