# Copyright (c) Johnny Patterson
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.26)

project(
	rtb
	VERSION 5.0.0
	LANGUAGES CXX
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Vulkan REQUIRED)

add_subdirectory(external)
add_subdirectory(launcher)
add_subdirectory(service)

add_dependencies(rtb-launcher rtb-service)
