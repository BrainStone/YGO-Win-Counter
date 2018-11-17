//
// Copyright (c) 2018 Yannick Schinko. All rights reserved.
// Licensed under the GPLv3 License. See LICENSE file in the project root for full license information.
//
#include <iostream>

#include "Replay.hpp"

int main() {
	Replay replay( "test.yrp" );

	std::basic_ofstream<byte> out( "decoded.yrp", std::ios_base::binary );

	out.write( replay.getData().c_str(), replay.getData().size() );

	out.close();
}
