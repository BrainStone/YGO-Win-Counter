//
// Copyright (c) 2018 Yannick Schinko. All rights reserved.
// Licensed under the GPLv3 License. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Filesystem.hpp"
#include <string>

#include <LzmaLib.h>

#include "Types.hpp"

class Replay {
public:
	static constexpr int REPLAY_COMPRESSED = 0x1;
	static constexpr int REPLAY_TAG = 0x2;
	static constexpr int REPLAY_DECODED = 0x4;
	static constexpr int REPLAY_SINGLE_MODE = 0x8;

	// Taken and adapted from
	// https://github.com/Fluorohydride/ygopro/blob/7f02a0000df562e1232e413dc354283ddc23996d/gframe/replay.h#L9-L22
	struct ReplayHeader {
		unsigned int id;
		unsigned int version;
		unsigned int flag;
		unsigned int seed;
		unsigned int datasize;
		unsigned int hash;
		byte props[8];
	};

	struct GameData {
		// Best buddies <3
		friend Replay;

		std::wstring playerName1;
		std::wstring playerName2;

		int lifePoints;
		int startHand;
		int drawCount;
		int options;

	protected:
		static std::wstring&& readPlayerName( const byte* data );
		static int readInt( const byte* data );

		virtual void readGameData( const byte* data );
	};

	class DecompressionError : public std::runtime_error {
	private:
		const int _code;

		static std::string getMessage( int code );

	public:
		explicit DecompressionError( int code );

		virtual int code() const noexcept;
	};

protected:
	ReplayHeader header;
	Blob data;

	GameData gameData;

public:
	explicit Replay( const std::filesystem::path& filename );


	virtual ReplayHeader& getHeader() noexcept;
	virtual Blob& getData() noexcept;

	virtual GameData& getGameData() noexcept;


	virtual const ReplayHeader& getHeader() const noexcept;
	virtual const Blob& getData() const noexcept;

	virtual const GameData& getGameData() const noexcept;

protected:
	virtual void loadDataFromFile( const std::filesystem::path& filename );

	virtual void loadFile( const std::filesystem::path& filename );
	virtual void uncompressData();
};
