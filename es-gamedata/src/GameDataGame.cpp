/*
 * GameDataGame.cpp
 *
 *  Created on: 5 Jan 2017
 *      Author: rhopkins
 */

#include "GameData.h"
#include "GameDataGame.h"
#include "GameDataException.h"
#include "GameDataMetadata.h"

GameDataGame::GameDataGame(sqlite3* db, const std::string& id, const std::string& systemID, const std::string& path,
						   int rating, int playCount ) :
	mID(id), mSystemID(systemID), mPath(path), mRating(rating), mPlayCount(playCount), GameDataItem()
{
	mTags = new GameDataTags(db, systemID, id);
	mMetadata = new GameDataMetadata(db, systemID, id);
}

GameDataGame::GameDataGame(sqlite3* db, sqlite3_stmt* row) : GameDataItem()
{
	// The row is the result of a query that gives us the game columns plus the name from
	// the metadata table
	const unsigned char* file_id 	= sqlite3_column_text(row, QUERY_GAMES_FILEID_COL);
	const unsigned char* system_id 	= sqlite3_column_text(row, QUERY_GAMES_SYSTEMID_COL);
	const unsigned char* path 		= sqlite3_column_text(row, QUERY_GAMES_PATH_COL);
	int rating 						= sqlite3_column_int(row, QUERY_GAMES_RATING_COL);
	int playCount 					= sqlite3_column_int(row, QUERY_GAMES_PLAYCOUNT_COL);
	const unsigned char* name		= sqlite3_column_text(row, QUERY_GAMES_NAME_COL);

	// file_id and system_id are mandatory
	if (!file_id || !system_id)
		throw GameDataException("Empty file_id or system_id when constructing game from database row");

	mID = (const char*)file_id;
	mSystemID = (const char*)system_id;
	if (path) mPath = (const char*)path;
	mRating = rating;
	mPlayCount = playCount;
	mName = name ? (const char*)name : mID;

	mTags = new GameDataTags(db, mSystemID, mID);
	mMetadata = new GameDataMetadata(db, mSystemID, mID);
}

GameDataGame::~GameDataGame()
{
	delete mTags;
	delete mMetadata;
}

