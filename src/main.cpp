//
// Copyright (c) 2018 Yannick Schinko. All rights reserved.
// Licensed under the GPLv3 License. See LICENSE file in the project root for full license information.
//
#include <bitset>
#include <iomanip>
#include <iostream>

#include "Replay.hpp"

int main() {
	Replay replay( "test.yrp" );

	std::basic_ofstream<byte> out( "decoded.yrp", std::ios_base::binary );

	out.write( replay.getData().data(), replay.getData().size() );

	out.close();

	std::wcout << replay.getGameData().playerName1 << std::endl;
	std::wcout << replay.getGameData().playerName2 << std::endl;
	std::wcout << replay.getGameData().lifePoints << std::endl;
	std::wcout << replay.getGameData().startHand << std::endl;
	std::wcout << replay.getGameData().drawCount << std::endl;
	std::wcout << std::hex << std::setfill(L'0') << std::fixed;
	std::wcout << std::setw( sizeof( uint16_t ) * 2 ) << replay.getGameData().options << std::endl;
	std::wcout << std::setw( sizeof( uint16_t ) * 2 ) << replay.getGameData().duelRules << std::endl;
	std::wcout << std::dec << std::setw( 1 ) << std::defaultfloat;
}
