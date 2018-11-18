//
// Copyright (c) 2018 Yannick Schinko. All rights reserved.
// Licensed under the GPLv3 License. See LICENSE file in the project root for full license information.
//
#pragma once

// We haven't checked which filesystem to include yet
#ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL

// We can't check if headers exist...
// Let's assume experimental to be safe
#	if !defined(__has_include)
#		define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1

// Check if the header "<filesystem>" exists
#	elif __has_include(<filesystem>)

// If we're compiling on Visual Studio and are not compiling with C++17, we need use experimental
#		if defined(_MSC_VER) && !(defined(_HAS_CXX17) && _HAS_CXX17)
#			define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1
#		else
#			define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0
#		endif

// Check if the header "<filesystem>" exists
#	elif __has_include(<experimental/filesystem>)
#		define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1

// Fail if neither header is available with a nice error message
#	else
#		error Could not find system header "<filesystem>" or "<experimental/filesystem>"
#	endif

// We priously determined that we need the exprimental version
#	if INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
// Include it
#		include <experimental/filesystem>

// We need the alias from std::experimental::filesystem to std::filesystem
namespace std {
	namespace filesystem = experimental::filesystem;
}

// We have a decent compiler and can use the normal version
#	else
// Include it
#		include <filesystem>
#	endif

#endif // #ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
