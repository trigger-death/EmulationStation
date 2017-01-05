/*
 * GameDataGame.cpp
 *
 *  Created on: 5 Jan 2017
 *      Author: rhopkins
 */

#include "GameData.h"
#include "GameDataGame.h"
#include "GameDataException.h"

GameDataGame::GameDataGame(sqlite3* db, const std::string& id, const std::string& systemID, const std::string& path,
						   int rating, int playCount ) :
	mID(id), mSystemID(systemID), mPath(path), mRating(rating), mPlayCount(playCount), GameDataItem()
{
	mTags = new GameDataTags(db, systemID, id);
}

GameDataGame::GameDataGame(sqlite3* db, sqlite3_stmt* row) : GameDataItem()
{
	const unsigned char* file_id 	= sqlite3_column_text(row, TABLE_GAMES_FILEID_COL);
	const unsigned char* system_id 	= sqlite3_column_text(row, TABLE_GAMES_SYSTEMID_COL);
	const unsigned char* path 		= sqlite3_column_text(row, TABLE_GAMES_PATH_COL);
	int rating 						= sqlite3_column_int(row, TABLE_GAMES_RATING_COL);
	int playCount 					= sqlite3_column_int(row, TABLE_GAMES_PLAYCOUNT_COL);

	// file_id and system_id are mandatory
	if (!file_id || !system_id)
		throw GameDataException("Empty file_id or system_id when constructing game from database row");

	mID = (const char*)file_id;
	mSystemID = (const char*)system_id;
	if (path) mPath = (const char*)path;
	mRating = rating;
	mPlayCount = playCount;

	mTags = new GameDataTags(db, mSystemID, mID);
}

GameDataGame::~GameDataGame()
{
	delete mTags;
}

