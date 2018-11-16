//
// Copyright (c) 2018 Yannick Schinko. All rights reserved.
// Licensed under the GPLv3 License. See LICENSE file in the project root for full license information.
//
#include "ReplayReader.hpp"

// Taken and adapted from
// https://github.com/Fluorohydride/ygopro/blob/7f02a0000df562e1232e413dc354283ddc23996d/gframe/replay.cpp#L141-L177

Replay openReplay( const std::filesystem::path& filename ) {
	Replay replay {};

	BinaryStream replayFile( filename, std::ios_base::binary );

	replayFile.read( reinterpret_cast<byte*>(&replay.header), sizeof( replay.header ) );
	replay.data = Blob( std::istreambuf_iterator<byte>( replayFile ), {} );

	replayFile.close();

	if ( replay.header.flag & ygo::REPLAY_COMPRESSED ) {
		size_t compressedSize = replay.data.size();
		size_t replaySize = replay.header.datasize;
		byte* replayData = new byte[replaySize];

		const int result = LzmaUncompress( replayData, &replaySize, replay.data.c_str(), &compressedSize, replay.header.props, 5 );

		if ( result != SZ_OK )
			throw DecompressionError( result );

		replay.data = Blob( replayData, replaySize );

		delete[] replayData;
	}

	return replay;
}
