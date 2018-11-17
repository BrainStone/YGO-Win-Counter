//
// Copyright (c) 2018 Yannick Schinko. All rights reserved.
// Licensed under the GPLv3 License. See LICENSE file in the project root for full license information.
//
#pragma once

#if __has_include(<filesystem>)
#	include <filesystem>
#elif __has_include(<experimental/filesystem>)
#	include <experimental/filesystem>

namespace std {
	namespace filesystem = experimental::filesystem;
}

#else
#	error Could not find system header \"<filesystem>\"
#endif
