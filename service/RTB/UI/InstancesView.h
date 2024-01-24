// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "RTB/UI/CreateInstanceView.h"

namespace RTB {
namespace UI {

struct Context;

class InstancesView
{
public:
	void Update(Context* ctx, bool* show, bool focus);

private:
	CreateInstanceView m_createInstance;

	bool m_showCreateInstance {};
	bool m_focusCreateInstance {};
};

} // namespace UI
} // namespace RTB
