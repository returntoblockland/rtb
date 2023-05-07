// Copyright (c) The rtb Contributors <https://github.com/returntoblockland/rtb>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "platform/platform.h"
#include "platform/platformVolume.h"
#include "app/mainLoop.h"
#include "core/util/journal/process.h"
#include "windowManager/platformWindowMgr.h"

static bool handleCommandLine(S32 argc, const char **argv);

S32 TorqueMain(S32 argc, const char **argv)
{
	StandardMainLoop::init();

	if (!handleCommandLine(argc, argv))
	{
		return 1;
	}

	while (StandardMainLoop::doMainLoop());

	StandardMainLoop::shutdown();

	return 0;
}

bool handleCommandLine(S32 argc, const char **argv)
{
	PlatformWindowManager::get()->processCmdLineArgs(argc, argv);
	Process::handleCommandLine(argc, argv);

	Con::setIntVariable("Game::argc", argc);
	for (S32 i = 0; i < argc; ++i)
		Con::setVariable(avar("Game::argv%d", 0), argv[i]);

	Platform::FS::InstallFileSystems();
	Platform::FS::MountDefaults();
	Torque::FS::SetCwd("game:/");
	Platform::setCurrentDirectory(Platform::getMainDotCsDir());

	return true;
}
