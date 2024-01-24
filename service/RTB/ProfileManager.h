// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "RTB/Profile.h"

#include <cstdint>
#include <mutex>
#include <vector>

namespace RTB {

class Scheduler;

class ProfileManager
{
public:
	static ProfileManager& Get();

	enum SteamProfileStatus
	{
		Unknown,
		Found,
		NoBlocklandInstall,
		NoSteamLibrary,
		NotImplemented,
	};

	int DetectSteamProfile();
	ProfileSPtr GetSteamProfile() const;
	int GetSteamProfileStatus() const;

private:
	ProfileSPtr m_steamProfile;
	int m_steamProfileStatus {};

	mutable std::mutex m_profilesMtx;
};

} // namespace RTB
