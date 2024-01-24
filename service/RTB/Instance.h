// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <memory>
#include <string>

namespace fs = std::filesystem;

namespace RTB {

class Profile;

using ProfileSPtr = std::shared_ptr<Profile>;
using ProfileWPtr = std::weak_ptr<Profile>;

class Instance
{
public:
	void Build();
	void ReadManifest(const fs::path& file);
	void WriteManifest(const fs::path& file);

	const std::string& GetName() const { return m_name; }
	void SetName(std::string_view name) { m_name = name; }
	ProfileSPtr GetProfile() const { return m_profile.lock(); }
	void SetProfile(const ProfileSPtr& profile) { m_profile = profile; }
	const fs::path& GetProfileDirectory() const { return m_profileDirectory; }
	void SetProfileDirectory(const fs::path& directory) { m_profileDirectory = directory; }

private:
	std::string m_name;
	fs::path m_profileDirectory;
	ProfileWPtr m_profile;
};

using InstanceSPtr = std::shared_ptr<Instance>;
using InstanceWPtr = std::weak_ptr<Instance>;

} // namespace RTB
