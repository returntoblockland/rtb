// Copyright (c) The rtb Contributors <https://github.com/returntoblockland/rtb>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "platform/platform.h"
#include "platform/platformVolume.h"
#include "app/mainLoop.h"
#include "core/util/journal/process.h"
#include "gui/core/guiCanvas.h"
#include "windowManager/platformWindowMgr.h"

static bool handleCommandLine(S32 argc, const char **argv);
static void init();

S32 TorqueInit(S32 argc, const char **argv)
{
	StandardMainLoop::init();

	if (!handleCommandLine(argc, argv))
	{
		return 1;
	}

	Con::setLogMode(6);

	init();

    return 0;
}

bool TorqueTick()
{
	return StandardMainLoop::doMainLoop();
}

S32 TorqueShutdown(S32 exitCode)
{
	StandardMainLoop::shutdown();
	return exitCode;
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

void init()
{
	GuiControlProfile* defaultProfile = new GuiControlProfile();
	defaultProfile->registerObject("GuiDefaultProfile");

	GuiControlProfile* tooltipProfile = new GuiControlProfile();
	tooltipProfile->registerObject("GuiTooltipProfile");

	GuiCanvas* canvas = new GuiCanvas();
	canvas->registerObject("Canvas");
}
