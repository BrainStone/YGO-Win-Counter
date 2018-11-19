//
// Copyright (c) 2018 Yannick Schinko. All rights reserved.
// Licensed under the GPLv3 License. See LICENSE file in the project root for full license information.
//
#include "Replay.hpp"

std::wstring&& Replay::GameData::readPlayerName( const byte* data ) {
	std::wstring result( reinterpret_cast<const wchar_t*>(data) );

	if ( result.length() > 20 )
		result.substr( 0, 20 );

	return std::move( result );
}

int Replay::GameData::readInt( const byte* data ) {
	return *reinterpret_cast<const int*>(data);
}

void Replay::GameData::readGameData( const byte* data ) {
	size_t offset = 0;

	playerName1 = readPlayerName( data + offset ); offset += 40;
	playerName2 = readPlayerName( data + offset ); offset += 40;

	lifePoints = readInt( data + offset ); offset += 4;
	startHand = readInt( data + offset ); offset += 4;
	drawCount = readInt( data + offset ); offset += 4;
	options = readInt( data + offset ); offset += 4;
}

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

Replay::GameData& Replay::getGameData() noexcept {
	return gameData;
}

const Replay::ReplayHeader& Replay::getHeader() const noexcept {
	return header;
}

const Blob& Replay::getData() const noexcept {
	return data;
}

const Replay::GameData& Replay::getGameData() const noexcept {
	return gameData;
}

void Replay::loadDataFromFile( const std::filesystem::path & filename ) {
	loadFile( filename );

	if ( header.flag & REPLAY_COMPRESSED ) {
		uncompressData();
	}

	gameData.readGameData( data.data() );
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
