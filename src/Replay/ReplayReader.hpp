//
// Copyright (c) 2018 Yannick Schinko. All rights reserved.
// Licensed under the GPLv3 License. See LICENSE file in the project root for full license information.
//
#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include <LzmaLib.h>

#include "ReplayHeader.hpp"

using byte = uint8_t;
using Blob = std::basic_string<byte>;

struct Replay {
	ygo::ReplayHeader header;
	Blob data;
};

Replay openReplay( const std::filesystem::path& filename );
