// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "RTB/UI/InstancesView.h"
#include "RTB/UI/ProfilesView.h"

namespace RTB {
namespace UI {

struct Context;

class HubView
{
public:
	void Update(Context* ctx);

private:
	InstancesView m_instances;
	ProfilesView m_profiles;

	bool m_showInstances { false };
	bool m_showProfiles { false };
};

} // namespace UI
} // namespace RTB
