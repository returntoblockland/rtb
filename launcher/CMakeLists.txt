# Copyright (c) Johnny Patterson

add_executable(rtb-launcher)
target_compile_features(rtb-launcher PRIVATE cxx_std_20)

file(GLOB_RECURSE src *)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${src})
target_sources(rtb-launcher PRIVATE
	${src}
)

if(MSVC)
	set_target_properties(rtb-launcher PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
	set_target_properties(rtb-launcher PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
