// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "App.h"

#ifdef _WIN32
#include <Shlobj.h>
#endif

namespace RTB {

fs::path App::GetDataDirectory()
{
#ifdef _WIN32
	TCHAR localAppDataDirectory[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, localAppDataDirectory)))
		return fs::path(localAppDataDirectory) / "ReturnToBlockland";
#endif

	return fs::current_path();
}

} // namespace RTB
