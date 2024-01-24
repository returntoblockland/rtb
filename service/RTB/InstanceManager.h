// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "RTB/Instance.h"

#include <filesystem>
#include <memory>
#include <mutex>
#include <vector>

namespace fs = std::filesystem;

namespace RTB {

class Profile;
class ProfileManager;
class Scheduler;

using ProfileSPtr = std::shared_ptr<Profile>;

class InstanceManager
{
public:
	using InstanceList = std::vector<InstanceSPtr>;

	void Create(Scheduler* scheduler, const ProfileSPtr& profile, std::string_view name);
	void DetectExistingInstances(const fs::path& directory, ProfileManager* profileManager);
	InstanceSPtr FindByName(std::string_view name) const;
	InstanceList GetInstances() const;
	bool IsBusy() const { return m_isBusy; }

private:
	std::vector<InstanceSPtr> m_instances;

	mutable std::mutex m_instancesMtx;

	bool m_isBusy {};
};

} // namespace RTB
