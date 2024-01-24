// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

namespace RTB {

class Profile
{
public:
	Profile(const fs::path& directory);

	const fs::path& GetDirectory() const { return m_directory; }

private:
	fs::path m_directory;
};

using ProfileSPtr = std::shared_ptr<Profile>;
using ProfileWPtr = std::weak_ptr<Profile>;

} // namespace RTB
