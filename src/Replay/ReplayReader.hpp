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
using BinaryStream = std::basic_ifstream<byte>;

struct Replay {
	ygo::ReplayHeader header;
	Blob data;
};

class DecompressionError : public std::runtime_error {
private:
	const int _code;

	static inline std::string getMessage( int code ) {
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

public:
	explicit inline DecompressionError( int code )
		: std::runtime_error( "Error while decompressing: " + getMessage( code ) ),
		_code( code ) {}

	virtual inline int code() const noexcept {
		return _code;
	}
};

Replay openReplay( const std::filesystem::path& filename );
