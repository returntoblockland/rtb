// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

namespace RTB {

class InstanceManager;
class ProfileManager;
class Scheduler;

namespace UI {

struct Context
{
	InstanceManager* InstanceManager;
	ProfileManager* ProfileManager;
	Scheduler* Scheduler;
};

} // namespace UI
} // namespace RTB
