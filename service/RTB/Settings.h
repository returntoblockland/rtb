// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace RTB {

class Settings
{
public:
	static constexpr std::string_view DefaultInstancesDirectory { "instances" };

	static Settings& Get();

	void Read();
	void Write();

	bool Dirty() const { return m_dirty; }

	void SetInstancesDirectory(const fs::path& p) { m_instancesDirectory = p; m_dirty = true; };
	const fs::path& GetInstancesDirectory() const { return m_instancesDirectory; }
	fs::path GetAbsoluteInstancesDirectory() const;

private:
	fs::path m_settingsFile;

	fs::path m_instancesDirectory { DefaultInstancesDirectory };

	bool m_dirty {};
};

} // namespace RTB
