// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Instance.h"

#include "RTB/Profile.h"
#include "RTB/Settings.h"

#include <nlohmann/json.hpp>

#include <fstream>

using json = nlohmann::json;

namespace RTB {

void Instance::Build()
{
	std::error_code ec;
	auto directory = Settings::Get().GetAbsoluteInstancesDirectory() / m_name;

	fs::create_directories(directory, ec);

	auto manifestFile = directory / "rtb.json";
	WriteManifest(manifestFile);

	// TODO: Trigger sync from profile.
}

void Instance::ReadManifest(const fs::path& file)
{
	std::ifstream in(file);
	if (!in.good())
		return;

	json manifest = json::parse(in);

	// TODO: Handle name mismatch. For now, assume name is the same as the directory.

	if (manifest.contains("profileDirectory"))
	{
		const auto& profileDirectory = manifest["profileDirectory"];
		if (profileDirectory.is_string())
			m_profileDirectory = profileDirectory.get<std::string>();
	}
}

void Instance::WriteManifest(const fs::path& file)
{
	json manifest;

	manifest["name"] = m_name;
	manifest["profileDirectory"] = m_profileDirectory.generic_string();

	if (file.has_parent_path())
	{
		std::error_code ec;
		fs::create_directories(file.parent_path(), ec);
	}

	std::ofstream out(file);
	out << manifest.dump(1, '\t');
}

} // namespace RTB
