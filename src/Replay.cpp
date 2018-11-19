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
	loadDataFromFile( filename );
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

void Replay::loadDataFromFile( const std::filesystem::path & filename ) {
	loadFile( filename );

	if ( header.flag & REPLAY_COMPRESSED ) {
		uncompressData();
	}
}

void Replay::loadFile( const std::filesystem::path & filename ) {
	ByteIFStream replayFile( filename, std::ios_base::binary | std::ios_base::ate );

	const size_t fileSize = replayFile.tellg();
	constexpr size_t headerSize = sizeof( header );
	const size_t dataSize = fileSize - headerSize;

	replayFile.seekg( 0 );
	data.resize( dataSize );

	replayFile.read( reinterpret_cast<byte*>(&header), headerSize );
	replayFile.read( reinterpret_cast<byte*>(data.data()), dataSize );

	replayFile.close();
}

void Replay::uncompressData() {
	size_t compressedSize = data.size();
	Blob compressedData = std::move( data );
	size_t replaySize = header.datasize;
	data = Blob( replaySize );

	const int result = LzmaUncompress( data.data(), &replaySize, compressedData.data(), &compressedSize, header.props, 5 );

	if ( result != SZ_OK )
		throw DecompressionError( result );
}
