# Copyright (c) The rtb Contributors <https://github.com/returntoblockland/rtb>
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.25)
project(
	rtb
	VERSION 0.1.0.0
)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif()

include(external/rtb-t3d/Engine/cmake/Scripts/setup_game.cmake)
include(external/rtb-t3d/Engine/cmake/Scripts/setup_nasm.cmake)

setup_nasm()
enable_language(ASM_NASM)

set(major ${PROJECT_VERSION_MAJOR})
set(minor ${PROJECT_VERSION_MINOR})
set(patch ${PROJECT_VERSION_PATCH})
set(tweak ${PROJECT_VERSION_TWEAK})
set(TORQUE_APP_NAME "Return to Blockland")
set(TORQUE_APP_VERSION ${major}${minor}${patch}${tweak})
configure_file(source/torqueConfig.h.in torqueConfig.h @ONLY)
source_group("" FILES ${CMAKE_CURRENT_BINARY_DIR}/torqueConfig.h)

set(ENGINE_NAME ${PROJECT_NAME})
set(ENGINE_HIFI_NET FALSE)
add_subdirectory(external/rtb-t3d/Engine Engine)

add_executable(${PROJECT_NAME} source/main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER rtb)
target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		source
)
target_sources(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/torqueConfig.h
)

setup_game(${PROJECT_NAME} ${ENGINE_NAME})

if(WIN32)
	set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE TRUE)
endif()
