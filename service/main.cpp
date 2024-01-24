// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "RTB/InstanceManager.h"
#include "RTB/ProfileManager.h"
#include "RTB/Scheduler.h"
#include "RTB/Settings.h"
#include "RTB/UI/Context.h"
#include "RTB/UI/ManagerApp.h"

#include <csignal>
#include <cstdlib>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

static void SignalHandler(int signal);

#ifdef _WIN32
static BOOL WINAPI SignalHandler(DWORD signal);
#endif

using namespace RTB;

int main(int argc, char* argv[])
{
	std::signal(SIGABRT, SignalHandler);
	std::signal(SIGINT, SignalHandler);
	std::signal(SIGSEGV, SignalHandler);
	std::signal(SIGTERM, SignalHandler);

#ifdef _WIN32
	SetConsoleCtrlHandler(SignalHandler, TRUE);
#endif

	auto& settings = Settings::Get();
	settings.Read();

	Scheduler scheduler(2, 8);
	InstanceManager instanceManager;
	ProfileManager profileManager;

	profileManager.DetectSteamProfile();
	instanceManager.DetectExistingInstances(settings.GetAbsoluteInstancesDirectory(), &profileManager);

	UI::Context uiContext;
	uiContext.InstanceManager = &instanceManager;
	uiContext.ProfileManager = &profileManager;
	uiContext.Scheduler = &scheduler;

	UI::ManagerApp managerApp(&uiContext);

	for (;;)
	{
		scheduler.MainQueue().Flush();

		if (managerApp.Stopped())
			break;

		std::this_thread::yield();
	}
}

static void SignalHandler(int signal)
{
	std::quick_exit(0);
}

#ifdef _WIN32
static BOOL SignalHandler(DWORD signal)
{
	std::quick_exit(0);
	return FALSE;
}
#endif
