// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Settings.h"

#include "RTB/App.h"

#include <nlohmann/json.hpp>

#include <fstream>

using json = nlohmann::json;

namespace RTB {

Settings& Settings::Get()
{
	static Settings sl_settings;

	if (sl_settings.m_settingsFile.empty())
		sl_settings.m_settingsFile = App::GetDataDirectory() / "settings.json";

	return sl_settings;
}

void Settings::Read()
{
	std::ifstream in(m_settingsFile);
	if (!in.good())
	{
		m_dirty = true;
		return;
	}

	json settings = json::parse(in);

	if (settings.contains("instancesDirectory"))
	{
		const auto& instancesDirectory = settings["instancesDirectory"];
		if (instancesDirectory.is_string())
			m_instancesDirectory = instancesDirectory.get<std::string>();
	}
}

void Settings::Write()
{
	json settings;

	settings["instancesDirectory"] = (const char*)m_instancesDirectory.u8string().c_str();

	if (m_settingsFile.has_parent_path())
	{
		std::error_code ec;
		fs::create_directories(m_settingsFile.parent_path(), ec);
	}

	std::ofstream out(m_settingsFile);
	out << settings.dump(1, '\t');

	m_dirty = false;
}

fs::path Settings::GetAbsoluteInstancesDirectory() const
{
	return m_instancesDirectory.is_relative() ? (App::GetDataDirectory() / m_instancesDirectory) : m_instancesDirectory;
}

} // namespace RTB
