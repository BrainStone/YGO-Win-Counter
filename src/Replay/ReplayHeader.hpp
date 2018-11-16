//
// Copyright (c) 2018 Yannick Schinko. All rights reserved.
// Licensed under the GPLv3 License. See LICENSE file in the project root for full license information.
//
#pragma once

// Taken and adapted from
// https://github.com/Fluorohydride/ygopro/blob/7f02a0000df562e1232e413dc354283ddc23996d/gframe/replay.h#L9-L22

namespace ygo {
	constexpr int REPLAY_COMPRESSED = 0x1;
	constexpr int REPLAY_TAG = 0x2;
	constexpr int REPLAY_DECODED = 0x4;
	constexpr int REPLAY_SINGLE_MODE = 0x8;

	struct ReplayHeader {
		unsigned int id;
		unsigned int version;
		unsigned int flag;
		unsigned int seed;
		unsigned int datasize;
		unsigned int hash;
		unsigned char props[8];
	};
}
