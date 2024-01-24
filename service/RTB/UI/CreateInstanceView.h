// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

namespace RTB {
namespace UI {

struct Context;

class CreateInstanceView
{
public:
	void Update(Context* ctx, bool* show, bool focus);

private:
	static constexpr size_t NameInputSize = 64;
	char m_nameInput[NameInputSize] {};
};

} // namespace UI
} // namespace RTB
