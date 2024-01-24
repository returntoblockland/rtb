// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace RTB {

class App
{
public:
	static fs::path GetDataDirectory();
};

} // namespace RTB
