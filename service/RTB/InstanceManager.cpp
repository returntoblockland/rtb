// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InstanceManager.h"

#include "RTB/Profile.h"
#include "RTB/ProfileManager.h"
#include "RTB/Scheduler.h"

namespace RTB {

void InstanceManager::Create(Scheduler* scheduler, const ProfileSPtr& profile, std::string_view name)
{
	auto instance = std::make_shared<Instance>();
	instance->SetName(name);
	instance->SetProfile(profile);

	if (profile)
		instance->SetProfileDirectory(profile->GetDirectory());

	m_instancesMtx.lock();
	m_instances.push_back(instance);
	m_instancesMtx.unlock();

	tf::Taskflow tf;

	tf.emplace(
		[&, instance]() {
			instance->Build();
			m_isBusy = false;
		}
	);

	m_isBusy = true;
	scheduler->Run(std::move(tf));
}

void InstanceManager::DetectExistingInstances(const fs::path& directory, ProfileManager* profileManager)
{
	std::error_code ec;

	for (const auto& entry : fs::directory_iterator(directory, ec))
	{
		if (!entry.is_directory())
			continue;

		auto manifestFile = entry.path() / "rtb.json";

		if (fs::exists(manifestFile, ec))
		{
			auto instance = std::make_shared<Instance>();
			instance->SetName(entry.path().stem().generic_string());
			instance->ReadManifest(manifestFile);

			const auto& profileDirectory = instance->GetProfileDirectory();
			if (auto steamProfile = profileManager->GetSteamProfile())
				if (steamProfile->GetDirectory().generic_string() == profileDirectory.generic_string())
					instance->SetProfile(steamProfile);

			m_instances.push_back(instance);
		}
	}
}

InstanceSPtr InstanceManager::FindByName(std::string_view name) const
{
	std::lock_guard<std::mutex> lk(m_instancesMtx);

	for (const auto& instance : m_instances)
		if (instance->GetName() == name)
			return instance;

	return {};
}

auto InstanceManager::GetInstances() const -> InstanceList
{
	std::lock_guard<std::mutex> lk(m_instancesMtx);
	return m_instances;
}

} // namespace RTB
