// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

namespace RTB {
namespace UI {

struct Context;

class ProfilesView
{
public:
	void Update(Context* ctx, bool* show, bool focus);
};

} // namespace UI
} // namespace RTB
