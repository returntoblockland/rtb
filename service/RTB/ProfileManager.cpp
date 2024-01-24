// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ProfileManager.h"

#include "RTB/App.h"

#include <nlohmann/json.hpp>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

#include <fstream>
#include <regex>
#include <string>

using json = nlohmann::json;

namespace RTB {

static std::wstring ConvertSteamToJson(const std::wstring& acf);
static int FindBlocklandSteamDirectory(fs::path& directory);
static bool ReadSteamFile(json& output, const fs::path& file);

#ifdef _WIN32
static bool GetWindowsRegistryString(std::wstring& result, HKEY key, const std::wstring& subkey, const std::wstring& value);
#endif

int ProfileManager::DetectSteamProfile()
{
	if (m_steamProfile)
	{
		// TODO: Confirm existing profile exists. If it moved, set new location. If it is missing, prevent creation of new instances and file sync.
		return SteamProfileStatus::Found;
	}

	fs::path blocklandSteamDirectory;
	auto status = FindBlocklandSteamDirectory(blocklandSteamDirectory);

	m_profilesMtx.lock();

	m_steamProfileStatus = status;
	m_steamProfileStatus = SteamProfileStatus::NotImplemented;

	if (status == SteamProfileStatus::Found)
		m_steamProfile = std::make_shared<Profile>(blocklandSteamDirectory);

	m_profilesMtx.unlock();

	return status;
}

ProfileSPtr ProfileManager::GetSteamProfile() const
{
	std::lock_guard<std::mutex> lk(m_profilesMtx);
	return m_steamProfile;
}

int ProfileManager::GetSteamProfileStatus() const
{
	std::lock_guard<std::mutex> lk(m_profilesMtx);
	return m_steamProfileStatus;
}

static std::wstring ConvertSteamToJson(const std::wstring& acf)
{
	// This should work for Steam's .acf and .vdf files.

	static const std::wregex header { LR"(^".+")" };
	static const std::wregex memberAssignment { L"\"\t\t\"" };
	static const std::wregex memberAssignmentLine { LR"(": ".*"$)" };
	static const std::wregex objectAssignment { L"\".+\"$" };
	static const std::wregex objectClosingBracket { L"^\t+\\}" };
	static const std::wregex trailingCommas { L",\r*\n*\t*\\}" };

	// Remove header.
	std::wstring result = std::regex_replace(acf, header, L"");

	// Format member assignments.
	result = std::regex_replace(result, memberAssignment, LR"(": ")");
	result = std::regex_replace(result, memberAssignmentLine, L"$&,");

	// Format objects.
	result = std::regex_replace(result, objectAssignment, L"$&:");
	result = std::regex_replace(result, objectClosingBracket, L"$&,");

	// Squish trailing commas.
	result = std::regex_replace(result, trailingCommas, L"}");

	return result;
}

static int FindBlocklandSteamDirectory(fs::path& directory)
{
#ifdef _WIN32
	std::wstring steamDirectory;

	if (!GetWindowsRegistryString(steamDirectory, HKEY_LOCAL_MACHINE, LR"(SOFTWARE\WOW6432Node\Valve\Steam)", L"InstallPath") &&
		!GetWindowsRegistryString(steamDirectory, HKEY_LOCAL_MACHINE, LR"(SOFTWARE\Valve\Steam)", L"InstallPath"))
	{
		return ProfileManager::SteamProfileStatus::NoSteamLibrary;
	}

	const fs::path libraryFoldersFile = fs::path(steamDirectory) / "steamapps" / "libraryfolders.vdf";
	json libraryFolders;
	std::error_code ec;

	if (!fs::exists(libraryFoldersFile, ec) || !ReadSteamFile(libraryFolders, libraryFoldersFile))
		return ProfileManager::SteamProfileStatus::NoBlocklandInstall;

	static constexpr char BlocklandAppManifestFile[] { "appmanifest_250340.acf" };
	fs::path appManifestFile;
	fs::path blocklandDirectory;
	fs::path libraryDirectory;
	json appManifest;

	try
	{
		for (const auto& item : libraryFolders.items())
		{
			for (const auto& appItem : item.value()["apps"].items())
			{
				libraryDirectory = fs::path(item.value()["path"].get<std::string>()) / "steamapps";

				appManifestFile = libraryDirectory / BlocklandAppManifestFile;

				if (!fs::exists(appManifestFile, ec) || !ReadSteamFile(appManifest, appManifestFile))
					continue;

				blocklandDirectory = libraryDirectory / "common" / appManifest["installdir"];
				if (!fs::exists(blocklandDirectory / "Blockland.exe", ec))
					continue;

				directory = blocklandDirectory;
				return ProfileManager::SteamProfileStatus::Found;
			}
		}
	}
	catch (...)
	{
	}

	return ProfileManager::SteamProfileStatus::NoBlocklandInstall;
#else
	return ProfileManager::SteamProfileStatus::NotImplemented;
#endif
}

static bool ReadSteamFile(json& output, const fs::path& file)
{
	try
	{
		std::wstring contents;
		std::wifstream stream(file, std::ios::in | std::ios::binary | std::ios::ate);
		size_t contentSize = stream.tellg();
		stream.seekg(0);
		contents.resize(contentSize);
		stream.read(contents.data(), contentSize);
		stream.close();
		contents = ConvertSteamToJson(contents);
		output = json::parse(contents);
	}
	catch (...)
	{
		return false;
	}

	return true;
}

#ifdef _WIN32
static bool GetWindowsRegistryString(std::wstring& result, HKEY key, const std::wstring& subkey, const std::wstring& value)
{
	DWORD size = 0;
	LSTATUS status;
	std::wstring str;

	status = RegGetValueW(key, subkey.c_str(), value.c_str(), RRF_RT_REG_SZ, NULL, NULL, &size);
	if (status != ERROR_SUCCESS)
		return false;

	str.resize(size / sizeof(wchar_t));
	status = RegGetValueW(key, subkey.c_str(), value.c_str(), RRF_RT_REG_SZ, NULL, &str[0], &size);
	if (status != ERROR_SUCCESS)
		return false;

	// Remove trailing null terminators.
	while (!str.back() && !str.empty())
		str.pop_back();

	result = str;
	return true;
}
#endif

} // namespace RTB
