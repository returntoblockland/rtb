// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "RTB/UI/HubView.h"

#include <thread>

namespace RTB {
namespace UI {

struct Context;

class ManagerApp
{
public:
	ManagerApp(Context* ctx);
	~ManagerApp();

	bool Stopped() const { return m_stopped; }

private:
	int UIThread();

	HubView m_hub;
	Context* m_ctx;
	std::thread m_uiThread;
	bool m_stopped {};
};

} // namespace UI
} // namespace RTB
