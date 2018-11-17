//
// Copyright (c) 2018 Yannick Schinko. All rights reserved.
// Licensed under the GPLv3 License. See LICENSE file in the project root for full license information.
//
#include "Replay.hpp"

std::string Replay::DecompressionError::getMessage( int code ) {
	switch ( code ) {
	case SZ_OK:
		return "OK";
	case SZ_ERROR_DATA:
		return "Data error";
	case SZ_ERROR_MEM:
		return "Memory allocation error";
	case SZ_ERROR_UNSUPPORTED:
		return "Unsupported properties";
	case SZ_ERROR_INPUT_EOF:
		return "Needs more bytes in input buffer( src )";
	default:
		return "Unknown error: " + std::to_string( code );
	}
}

Replay::DecompressionError::DecompressionError( int code )
	: std::runtime_error( "Error while decompressing: " + getMessage( code ) ),
	_code( code ) {}

int Replay::DecompressionError::code() const noexcept {
	return _code;
}

// Taken and adapted from
// https://github.com/Fluorohydride/ygopro/blob/7f02a0000df562e1232e413dc354283ddc23996d/gframe/replay.cpp#L141-L177

Replay::Replay( const std::filesystem::path& filename ) {
	BinaryStream replayFile( filename, std::ios_base::binary );

	replayFile.read( reinterpret_cast<byte*>(&header), sizeof( header ) );
	data = Blob( std::istreambuf_iterator<byte>( replayFile ), {} );

	replayFile.close();

	if ( header.flag & REPLAY_COMPRESSED ) {
		size_t compressedSize = data.size();
		size_t replaySize = header.datasize;
		byte* replayData = new byte[replaySize];

		const int result = LzmaUncompress( replayData, &replaySize, data.c_str(), &compressedSize, header.props, 5 );

		if ( result != SZ_OK )
			throw DecompressionError( result );

		data = Blob( replayData, replaySize );

		delete[] replayData;
	}
}

Replay::ReplayHeader& Replay::getHeader() noexcept {
	return header;
}

Blob& Replay::getData() noexcept {
	return data;
}

const Replay::ReplayHeader& Replay::getHeader() const noexcept {
	return header;
}

const Blob& Replay::getData() const noexcept {
	return data;
}